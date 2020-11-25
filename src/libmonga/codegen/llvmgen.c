#include "mon_codegen.h"

#include <stdarg.h>
#include <setjmp.h>
#include <string.h>
#include <monrt.h>

#include "mon_dict.h"
#include "mon_alloc.h"
#include "mon_debug.h"
#include "../nameof.h"
#include "../sem/types.h"
#include "../ast_private.h"

#include "llvmctx.h"
#include "llvmemit.h"

//
//  Forward declarations:
//
static void CompileBlock(LlvmGenContext* ctx, Mon_AstBlock* block);
static LlvmValue CompileVar(LlvmGenContext* ctx, Mon_AstVar* var);
static LlvmValue CompileCall(LlvmGenContext* ctx, Mon_AstCall* call);
static LlvmValue CompileExp(LlvmGenContext* ctx, Mon_AstExp* exp, Mon_AstTypeDef* targetType);
static void CompileCondition(LlvmGenContext* ctx, Mon_AstCond* cond, LlvmValue trueLabel, LlvmValue falseLabel);
//

/** 
 *  Compiles a local variable. The local variable data is added to the block context.
 */
static LlvmValue CompileLocal(LlvmGenContext* ctx, const char* name, Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(name);
    MON_CANT_BE_NULL(type);

    LocalVariableData* d = AddLocal(ctx, name, type);

    LlvmEmitAlloca(ctx, TypeToTypeRef(ctx, type, 0), d->location);

    return d->location;
}

static void CompileBinCond(LlvmGenContext* ctx, Mon_BinCondKind kind, 
                           Mon_AstCond* l, Mon_AstCond* r, 
                           LlvmValue trueLabel, LlvmValue falseLabel) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(l);
    MON_CANT_BE_NULL(r);

    LlvmValue nextLabel = ValLabel(ctx->blockCtx.nextLabel++);

    switch (kind) {
        case MON_BINCOND_AND:
            CompileCondition(ctx, l, nextLabel, falseLabel);
            LlvmEmitLabel(ctx, nextLabel);
            CompileCondition(ctx, r, trueLabel, falseLabel);
            return;

        case MON_BINCOND_OR:
            CompileCondition(ctx, l, trueLabel, nextLabel);
            LlvmEmitLabel(ctx, nextLabel);
            CompileCondition(ctx, r, trueLabel, falseLabel);
            return;
    }
    
    MON_UNREACHABLE();
}

static void CompileComparison(LlvmGenContext* ctx, Mon_AstCond* cond, LlvmValue trueLabel, LlvmValue falseLabel) {
    MON_CANT_BE_NULL(cond);
    MON_CANT_BE_NULL(ctx);
    MON_ASSERT(cond->condKind == MON_COND_COMPARISON,
        "Specified condition node must be a comparison. (got %d)", 
        (int)cond->condKind);

    // Decide comparison type
    // When comparing two expressions, the expression to the right will
    // first get converted to the same type as the expression to the left.
    Mon_AstTypeDef* rType = cond->condition.compar.right->semantic.type;
    Mon_AstTypeDef* lType = cond->condition.compar.left->semantic.type;

    MON_ASSERT(TypeCanCompare(lType, rType, cond->condition.compar.comparKind),
        "lType and rType must be comparable.");

    LlvmValue lExpLoc = CompileExp(ctx, cond->condition.compar.left, lType);
    LlvmValue rExpLoc = CompileExp(ctx, cond->condition.compar.right, lType);
    
    // Now that the conversion has been made, we need to specify the correct
    // LLVM comparison operator to use based on the expressions' types.
    LlvmComparKind comparKind;
    if (IsIntegerType(lType)) {
        switch (cond->condition.compar.comparKind) {
            case MON_COMPAR_GT:
                comparKind = LLVM_CMP_SGT;
                break;

            case MON_COMPAR_GE:
                comparKind = LLVM_CMP_SGE;
                break;

            case MON_COMPAR_EQ:
                comparKind = LLVM_CMP_EQ;
                break;

            case MON_COMPAR_NE:
                comparKind = LLVM_CMP_NEQ;
                break;

            case MON_COMPAR_LE:
                comparKind = LLVM_CMP_SLE;
                break;

            case MON_COMPAR_LT:
                comparKind = LLVM_CMP_SLT;
                break;
        }
    } else if (IsFloatingPointType(lType)) {
        switch (cond->condition.compar.comparKind) {
            case MON_COMPAR_GT:
                comparKind = LLVM_CMP_OGT;
                break;

            case MON_COMPAR_GE:
                comparKind = LLVM_CMP_OGE;
                break;

            case MON_COMPAR_EQ:
                comparKind = LLVM_CMP_OEQ;
                break;

            case MON_COMPAR_NE:
                comparKind = LLVM_CMP_UNEQ;
                break;

            case MON_COMPAR_LE:
                comparKind = LLVM_CMP_OLE;
                break;

            case MON_COMPAR_LT:
                comparKind = LLVM_CMP_OLT;
                break;
        }
    } else {
        // Other types can only have equality or inequality comparisons.
        if (cond->condition.compar.comparKind == MON_COMPAR_EQ) {
            comparKind = LLVM_CMP_EQ;
        } else if (cond->condition.compar.comparKind == MON_COMPAR_NE) {
            comparKind = LLVM_CMP_NEQ;
        } else {
            MON_UNREACHABLE();
        }
    }

    // Comparison kind selected, emit the comparison and branch instructions.
    LlvmTypeRef lTypeRef = TypeToTypeRef(ctx, lType, 0);
    LlvmValue boolexpr = LlvmEmitComparison(ctx, lTypeRef, lExpLoc, rExpLoc, comparKind);
    LlvmEmitCondBranch(ctx, boolexpr, trueLabel, falseLabel);
}

static void CompileCondition(LlvmGenContext* ctx, Mon_AstCond* cond, LlvmValue trueLabel, LlvmValue falseLabel) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(cond);

    if (cond->negate) {
        LlvmValue temp = trueLabel;
        trueLabel = falseLabel;
        falseLabel = temp;
    }

    switch (cond->condKind) {
        case MON_COND_BIN:
            CompileBinCond(ctx, cond->condition.binCond.binCondKind, 
                           cond->condition.binCond.left, cond->condition.binCond.right,
                           trueLabel, falseLabel);
            break;

        case MON_COND_COMPARISON:
            CompileComparison(ctx, cond, trueLabel, falseLabel);
            break;
    }
}

static LlvmValue CompileBinop(LlvmGenContext* ctx, 
                              Mon_AstTypeDef* targetType, 
                              Mon_AstExp* l, Mon_AstExp* r, 
                              Mon_BinopKind binop) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(targetType);
    MON_CANT_BE_NULL(l);
    MON_CANT_BE_NULL(r);

    LlvmTypeRef targetTypeRef = TypeToTypeRef(ctx, targetType, 0);

    LlvmValue lLoc = CompileExp(ctx, l, targetType);
    LlvmValue rLoc = CompileExp(ctx, r, targetType);
    
    if (IsFloatingPointType(targetType)) {
        switch (binop) {   
            case MON_BINOP_ADD:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_FADD);

            case MON_BINOP_SUB:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_FSUB);
                
            case MON_BINOP_MUL:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_FMUL);

            case MON_BINOP_DIV:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_FDIV);

            case MON_BINOP_MODULO:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_FREM);

            case MON_BINOP_SHR:
            case MON_BINOP_SHL:
            case MON_BINOP_BITAND:
            case MON_BINOP_BITOR:
            case MON_BINOP_XOR:
                MON_UNREACHABLE();
                return ValLocal(-1);
        }
    } else if (IsIntegerType(targetType)) {
        switch (binop) {   
            case MON_BINOP_ADD:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_ADD);

            case MON_BINOP_SUB:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_SUB);

            case MON_BINOP_MUL:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_MUL);

            case MON_BINOP_DIV:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_SDIV);

            case MON_BINOP_MODULO:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_SREM);

            case MON_BINOP_SHR:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_LSHR);

            case MON_BINOP_SHL:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_SHL);

            case MON_BINOP_BITAND:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_AND);

            case MON_BINOP_BITOR:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_OR);

            case MON_BINOP_XOR:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_XOR);
        }
    }

    MON_UNREACHABLE();
    return ValLocal(-1);
}

static LlvmValue CompileFloatToFloat(LlvmGenContext* ctx,
                                        Mon_AstTypeDef* expType,
                                        LlvmValue expLoc,
                                        Mon_AstTypeDef* destType) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(expType);
    MON_CANT_BE_NULL(destType);
    MON_ASSERT(IsFloatingPointType(expType) && IsFloatingPointType(destType),
                "Bad usage of compile float to float conversion (tried converting %d to %d).",
                (int)expType->typeDesc->typeDesc.primitive.typeCode,
                (int)destType->typeDesc->typeDesc.primitive.typeCode);

    expType = GetUnderlyingType(expType);
    destType = GetUnderlyingType(destType);

    Mon_PrimitiveTypeCode expTypeCode = expType->typeDesc->typeDesc.primitive.typeCode;
    Mon_PrimitiveTypeCode destTypeCode = destType->typeDesc->typeDesc.primitive.typeCode;

    if (expTypeCode == destTypeCode) {
        return expLoc;
    }

    if (expTypeCode == MON_PRIMITIVE_FLOAT32 &&
        destTypeCode == MON_PRIMITIVE_FLOAT64) {
        // Extend floating point
        return LlvmEmitFpext(ctx, TypeToTypeRef(ctx, expType, 0), 
                            expLoc, TypeToTypeRef(ctx, destType, 0));
    }

    if (expTypeCode == MON_PRIMITIVE_FLOAT64 &&
        destTypeCode == MON_PRIMITIVE_FLOAT32) {
        // Truncate floating point
        return LlvmEmitFptrunc(ctx, TypeToTypeRef(ctx, expType, 0), 
                              expLoc, TypeToTypeRef(ctx, destType, 0));        
    }

    MON_UNREACHABLE();
    return ValLocal(-1);
}

static LlvmValue CompileIntToInt(LlvmGenContext* ctx,
                                    Mon_AstTypeDef* expType,
                                    LlvmValue expLoc,
                                    Mon_AstTypeDef* destType) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(expType);
    MON_CANT_BE_NULL(destType);
    MON_ASSERT(IsIntegerType(expType) && IsIntegerType(destType),
                "Bad usage of compile int to int conversion (tried converting %d to %d).",
                (int)expType->typeDesc->typeDesc.primitive.typeCode,
                (int)destType->typeDesc->typeDesc.primitive.typeCode);

    expType = GetUnderlyingType(expType);
    destType = GetUnderlyingType(destType);

    Mon_PrimitiveTypeCode expTypeCode = expType->typeDesc->typeDesc.primitive.typeCode;
    Mon_PrimitiveTypeCode destTypeCode = destType->typeDesc->typeDesc.primitive.typeCode;

    int expSize = Mon_GetPrimitiveSize(expTypeCode);
    int destSize = Mon_GetPrimitiveSize(destTypeCode);

    if (expSize == destSize) {
        return expLoc;
    }
    if (expSize < destSize) {
        return LlvmEmitSext(ctx, TypeToTypeRef(ctx, expType, 0), 
                            expLoc, TypeToTypeRef(ctx, destType, 0));
    }
    return LlvmEmitTrunc(ctx, TypeToTypeRef(ctx, expType, 0), 
                         expLoc, TypeToTypeRef(ctx, destType, 0));
}

static LlvmValue CompileConversion(LlvmGenContext* ctx, 
                                   Mon_AstTypeDef* expType, 
                                   LlvmValue expLoc, 
                                   Mon_AstTypeDef* destType) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(expType);
    MON_CANT_BE_NULL(destType);

    expType = GetUnderlyingType(expType);
    destType = GetUnderlyingType(destType);

    if (expType == destType) {
        // Expression type is the same as the target type,
        // no need to perform conversions.
        return expLoc;
    }

    if (expType == BUILTIN_TABLE->types.tNull && IsRefType(destType)) {
        return ValNull();
    }

    if (destType == BUILTIN_TABLE->types.tString &&
        expType->typeDesc->typeDescKind == MON_TYPEDESC_ARRAY &&
        GetUnderlyingType(expType->typeDesc->typeDesc.array.semantic.innerTypeDef) == BUILTIN_TABLE->types.tChar) {
        LlvmEmit(ctx, "\t");

        // Get array pointer
        LlvmValue ret = ValLocal(ctx->blockCtx.nextLocalId++);
        LlvmEmitValue(ctx, ret);
        LlvmEmit(ctx, " = call %%string* @" NAMEOF(RtInternal_StrFromSZ) "(i8* ");
        LlvmEmitValue(ctx, expLoc);
        LlvmEmit(ctx, ")\n");
        return ret;
    }

    MON_ASSERT(IsTypeCastableFrom(destType, expType),
        "destType must be castable from expType.");     
         
    // We are assuming that the current rule for type casting
    // is to allow if and only if both types are primitives and
    // they aren't a 'char' + 'floatx' pair.
    // If one day we semantically add new types of conversion
    // and forget to implement them here, this assertion will
    // cause an error.
    MON_ASSERT(expType->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE &&
               destType->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE,
               "Conversion codegen assumes both types are primitives.");

    MON_ASSERT(expType->typeDesc->typeDesc.primitive.typeCode != MON_PRIMITIVE_VOID &&
               destType->typeDesc->typeDesc.primitive.typeCode != MON_PRIMITIVE_VOID,
               "expType and destType cannot hava void primitive type code.");

    if (IsIntegerType(expType)) {
        MON_ASSERT(IsNumericType(destType), 
            "Conversion codegen assumes integers can only be converted to other numeric types.");

        if (IsIntegerType(destType)) {
            return CompileIntToInt(ctx, expType, expLoc, destType);
        }

        return LlvmEmitSitofp(ctx, TypeToTypeRef(ctx, expType, 0),
                                expLoc, TypeToTypeRef(ctx, destType, 0));
    }

    if (IsFloatingPointType(destType)) {
        MON_ASSERT(destType->typeDesc->typeDesc.primitive.typeCode != MON_PRIMITIVE_CHAR, 
            "Conversion codegen assumes floats cannot be converted to chars.");

        if (IsFloatingPointType(destType)) {
            return CompileFloatToFloat(ctx, expType, expLoc, destType);
        }

        return LlvmEmitFptosi(ctx, TypeToTypeRef(ctx, expType, 0),
                                expLoc, TypeToTypeRef(ctx, destType, 0));
    }

    if (expType->typeDesc->typeDesc.primitive.typeCode == MON_PRIMITIVE_CHAR) {
        MON_ASSERT(!IsFloatingPointType(destType), 
            "Conversion codegen assumes chars cannot be converted to floats.");

        MON_ASSERT(Mon_GetPrimitiveSize(MON_PRIMITIVE_CHAR) == Mon_GetPrimitiveSize(MON_PRIMITIVE_INT8),
            "Assumes chars are the size of an int8.");
            
        return CompileConversion(ctx, BUILTIN_TABLE->types.tByte, expLoc, destType);
    }

    MON_UNREACHABLE();
    return ValLocal(-1);
}

static LlvmValue GetDefaultValue(Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    type = GetUnderlyingType(type);

    if (IsFloatingPointType(type)) {
        Mon_Literal lit;
        lit.literalKind = MON_LIT_FLOAT;
        lit.real = 0;
        return ValLiteral(lit);        
    }

    if (type->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE) {
        Mon_Literal lit;
        lit.literalKind = MON_LIT_INT;
        lit.integer = 0;
        return ValLiteral(lit);
    }

    if (IsRefType(type)) {
        return ValNull();
    }

    MON_UNREACHABLE();
    return ValNull();
}

static LlvmValue CompileNew(LlvmGenContext* ctx, Mon_AstTypeDef* type, Mon_AstExp* sizeExp) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(type);

    LlvmValue sizeExpLoc;

    if (sizeExp != NULL) {
        LlvmValue loc = CompileExp(ctx, sizeExp, BUILTIN_TABLE->types.tInt);
        Mon_Literal lit;
        lit.integer = GetTypeSize(type->typeDesc->typeDesc.array.semantic.innerTypeDef);
        lit.literalKind = MON_LIT_INT;
        sizeExpLoc = LlvmEmitBinop(ctx, TypeToTypeRef(ctx, BUILTIN_TABLE->types.tInt, 0), loc, ValLiteral(lit), LLVM_BINOP_MUL);
    } else {
        Mon_Literal lit;
        lit.integer = GetRecordDescSize(type->typeDesc);
        lit.literalKind = MON_LIT_INT;
        sizeExpLoc = ValLiteral(lit);
    }

    // Array new
    LlvmValue ptrVal = ValLocal(ctx->blockCtx.nextLocalId++);
    LlvmEmit(ctx, "\t");
    LlvmEmitValue(ctx, ptrVal);
    LlvmEmit(ctx, " = call i8* @" NAMEOF(RtInternal_GcAlloc) "(i32 ");
    LlvmEmitValue(ctx, sizeExpLoc);
    LlvmEmit(ctx, ")\n");
    LlvmValue expLoc = ValLocal(ctx->blockCtx.nextLocalId++);
    LlvmEmit(ctx, "\t");
    LlvmEmitValue(ctx, expLoc);
    LlvmEmit(ctx, " = bitcast i8* ");
    LlvmEmitValue(ctx, ptrVal);
    LlvmEmit(ctx, " to ");
    LlvmEmitTyperef(ctx, TypeToTypeRef(ctx, type, 0));
    LlvmEmit(ctx, "\n");

    return expLoc;
}

static LlvmValue CompileUnop(LlvmGenContext* ctx, 
                             Mon_AstExp* exp, 
                             Mon_UnopKind unop) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(exp);
    
    LlvmValue expLoc = CompileExp(ctx, exp, exp->semantic.type);

    switch (unop) {
        case MON_UNOP_NEGATIVE: {
            LlvmValue ret = ValLocal(ctx->blockCtx.nextLocalId++);

            Mon_Literal litZero;
            litZero.integer = 0;
            litZero.literalKind = MON_LIT_INT;
            ret = LlvmEmitBinop(ctx, TypeToTypeRef(ctx, exp->semantic.type, 0),
                                ValLiteral(litZero), expLoc, LLVM_BINOP_SUB);
            return ret;
        }

        case MON_UNOP_BITNOT:
            break;

        case MON_UNOP_LEN:
            break;
    }

    MON_UNREACHABLE();
    return ValLocal(-1);
}

/**
 *  Compiles an expression and returns its locid.
 */
static LlvmValue CompileExp(LlvmGenContext* ctx, Mon_AstExp* exp, Mon_AstTypeDef* targetType) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(exp);

    LlvmValue expLoc;

    switch (exp->expKind) {
        case MON_EXP_CONDITIONAL: {
            LlvmValue trueLabel = ValLabel(ctx->blockCtx.nextLabel++);
            LlvmValue falseLabel = ValLabel(ctx->blockCtx.nextLabel++);
            LlvmValue endLabel = ValLabel(ctx->blockCtx.nextLabel++);
            CompileCondition(ctx, exp->exp.conditionalExpr.condition, trueLabel, falseLabel);
            
            LlvmEmitLabel(ctx, trueLabel);
            LlvmValue trueValue = CompileExp(ctx, exp->exp.conditionalExpr.thenExpr, exp->semantic.type);
            LlvmValue trueLabelEnd = ValLabel(ctx->blockCtx.nextLabel++);
            LlvmEmitBranch(ctx, trueLabelEnd);
            LlvmEmitLabel(ctx, trueLabelEnd);
            LlvmEmitBranch(ctx, endLabel);
            
            LlvmEmitLabel(ctx, falseLabel);
            LlvmValue falseValue = CompileExp(ctx, exp->exp.conditionalExpr.elseExpr, exp->semantic.type);
            LlvmValue falseLabelEnd = ValLabel(ctx->blockCtx.nextLabel++);
            LlvmEmitBranch(ctx, falseLabelEnd);
            LlvmEmitLabel(ctx, falseLabelEnd);
            LlvmEmitBranch(ctx, endLabel);
            LlvmEmitLabel(ctx, endLabel);

            expLoc = LlvmEmitPhi(ctx, TypeToTypeRef(ctx, exp->semantic.type, 0), 
                                 trueLabelEnd, trueValue, 
                                 falseLabelEnd, falseValue);
            break;
        }

        case MON_EXP_UNOP:
            expLoc = CompileUnop(ctx, exp->exp.unaryOperation.operand, exp->exp.unaryOperation.unOpKind);
            break;

        case MON_EXP_BINOP:
            expLoc = CompileBinop(ctx, exp->semantic.type,
                                  exp->exp.binaryOperation.left, 
                                  exp->exp.binaryOperation.right, 
                                  exp->exp.binaryOperation.binOpKind);
            break;

        case MON_EXP_CAST:
            expLoc = CompileExp(ctx, exp->exp.castExpr.castee, exp->semantic.type);
            break;

        case MON_EXP_VAR: {
            LlvmValue varLoc = CompileVar(ctx, exp->exp.varExpr);
            LlvmValue ret = ValLocal(ctx->blockCtx.nextLocalId++);
            LlvmEmitLoad(ctx, TypeToTypeRef(ctx, exp->semantic.type, 0), varLoc, ret);
            expLoc = ret;
            break;
        }

        case MON_EXP_LITERAL:
            expLoc = ValLiteral(exp->exp.literalExpr);
            break;

        case MON_EXP_CALL:
            expLoc = CompileCall(ctx, exp->exp.callExpr);            
            break;

        case MON_EXP_NEW: {
            return CompileNew(ctx, exp->semantic.type, exp->exp.newExpr.arraySizeExp);
        }

        case MON_EXP_NULL:
            break;
    }

    targetType = GetUnderlyingType(targetType);
    Mon_AstTypeDef* expType = GetUnderlyingType(exp->semantic.type);
    if (targetType == expType) {
        return expLoc;
    }

    return CompileConversion(ctx, expType, expLoc, targetType);
}

/**
 *  Computes the address of an lvalue and returns its locid.
 */
static LlvmValue CompileVar(LlvmGenContext* ctx, Mon_AstVar* var) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(var);

    switch (var->varKind) {
        case MON_VAR_DIRECT: {
            LocalVariableData* d = FindLocal(ctx, var->var.direct.name);
            if (d != NULL) {
                // Variable is local, reuse its address locid.
                return d->location;
            }

            // Global variable, use its qualified name.
            return ValNamedGlobal(var->var.direct.name);
        }

        case MON_VAR_FIELD: {
            Mon_AstTypeDef* type = GetUnderlyingType(var->var.field.expr->semantic.type);
            MON_ASSERT(IsStructuredType(type), "Type must be structured.");

            LlvmValue expLoc = CompileExp(ctx, var->var.field.expr, type);
            
            LlvmTypeRef typeRef = TypeToTypeRef(ctx, type, 0);

            // Discover field index
            int fieldIdx = Mon_VectorGetIndex(&type->typeDesc->typeDesc.record.fields,
                                              var->var.field.semantic.field);
            
            if (fieldIdx == -1) {
                // Field should exist in type.
                MON_UNREACHABLE();
            }

            return LlvmEmitGetStructElementPtr(ctx, typeRef, expLoc, fieldIdx);
        }

        case MON_VAR_INDEXED: {
            Mon_AstTypeDef* elementType = var->semantic.type;

            LlvmValue indexExp = CompileExp(ctx, var->var.indexed.indexExpr, 
                                            BUILTIN_TABLE->types.tLong);
            LlvmValue indexedExp = CompileExp(ctx, var->var.indexed.indexedExpr, var->var.indexed.indexedExpr->semantic.type);
            return LlvmEmitGetArrayElementPtr(ctx, TypeToTypeRef(ctx, elementType, 0), indexedExp, indexExp);
        }
    }

    MON_UNREACHABLE();
    return ValLocal(-1);
}

static bool CompileIf(LlvmGenContext* ctx, 
                      Mon_AstCond* cond, 
                      Mon_AstBlock* thenBlock, 
                      Mon_AstBlock* elseBlock, 
                      LlvmValue thenLabel,
                      LlvmValue elseLabel) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(cond);
    MON_CANT_BE_NULL(thenBlock);

    CompileCondition(ctx, cond, thenLabel, elseLabel);

    // Compile 'then' block, since it will always exist.
    LlvmEmitLabel(ctx, thenLabel);
    CompileBlock(ctx, thenBlock);

    if (elseBlock != NULL) {
        LlvmValue endLabel;

        if (thenBlock->semantic.allPathsExit) {
            if (elseBlock->semantic.allPathsExit) {
                // Do not create an endLabel and compile else block.
                LlvmEmitLabel(ctx, elseLabel);
                CompileBlock(ctx, elseBlock);
                return true;
            }
            // Create end label, then does not branch, else does.
            endLabel = ValLabel(ctx->blockCtx.nextLabel++);
            LlvmEmitLabel(ctx, elseLabel);
            CompileBlock(ctx, elseBlock);
            LlvmEmitBranch(ctx, endLabel);
            return false;
        }

        if (elseBlock->semantic.allPathsExit) {
            // Create end label, then branches, else doesn't.
            endLabel = ValLabel(ctx->blockCtx.nextLabel++);
            LlvmEmitBranch(ctx, endLabel);
            LlvmEmitLabel(ctx, elseLabel);
            CompileBlock(ctx, elseBlock);
            return false;
        }

        // Create end label, then branches, else does too.
        endLabel = ValLabel(ctx->blockCtx.nextLabel++);
        LlvmEmitBranch(ctx, endLabel);
        LlvmEmitLabel(ctx, elseLabel);
        CompileBlock(ctx, elseBlock);
        LlvmEmitBranch(ctx, endLabel);
        LlvmEmitLabel(ctx, endLabel);
        return false;
    }

    if (thenBlock->semantic.allPathsExit) {
        LlvmEmitLabel(ctx, elseLabel);
        return false;
    }
    LlvmEmitBranch(ctx, elseLabel);
    LlvmEmitLabel(ctx, elseLabel);
    return false;
}

static bool CompileWhile(LlvmGenContext* ctx, 
                         Mon_AstCond* cond, 
                         Mon_AstBlock* body, 
                         LlvmValue endLabel) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(cond);
    MON_CANT_BE_NULL(body);

    LlvmValue loopBodyLabel = ValLabel(ctx->blockCtx.nextLabel++);
    LlvmValue loopCheckLabel = ValLabel(ctx->blockCtx.nextLabel++);
    CompileCondition(ctx, cond, loopBodyLabel, endLabel);
    LlvmEmitLabel(ctx, loopBodyLabel);

    LlvmValue prevEndLabel = ctx->enclosingLoopEndLabel;
    LlvmValue prevCheckLabel = ctx->enclosingLoopCheckLabel;
    ctx->enclosingLoopEndLabel = endLabel;
    ctx->enclosingLoopCheckLabel = loopCheckLabel;
    CompileBlock(ctx, body);
    ctx->enclosingLoopCheckLabel = prevCheckLabel;
    ctx->enclosingLoopEndLabel = prevEndLabel;

    if (!body->semantic.allPathsExit) {
        LlvmEmitBranch(ctx, loopCheckLabel);
    }

    LlvmEmitLabel(ctx, loopCheckLabel);
    CompileCondition(ctx, cond, loopBodyLabel, endLabel);
    LlvmEmitLabel(ctx, endLabel);

    return false;
}

/**
 *  Compiles a statement. Returns true if the statement always returns (and thus there is no point in compiling)
 *  other statements in a block.
 */
static bool CompileStatement(LlvmGenContext* ctx, Mon_AstStatement* stmt) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(stmt);
    MON_CANT_BE_NULL(ctx->enclosingFunction);

    switch (stmt->statementKind) {
        case MON_STMT_IF: {
            LlvmValue thenLabel = ValLabel(ctx->blockCtx.nextLabel++);
            LlvmValue elseLabel = ValLabel(ctx->blockCtx.nextLabel++); 
            return CompileIf(ctx, stmt->statement.ifStmt.condition,
                                  stmt->statement.ifStmt.thenBlock,
                                  stmt->statement.ifStmt.elseBlock,
                                  thenLabel, elseLabel);
        }

        case MON_STMT_WHILE: {
            LlvmValue loopEndLabel = ValLabel(ctx->blockCtx.nextLabel++);
            return CompileWhile(ctx, stmt->statement.whileStmt.condition, 
                                stmt->statement.whileStmt.block,
                                loopEndLabel);
        }

        case MON_STMT_ASSIGNMENT: {
            LlvmValue varLoc = CompileVar(ctx, stmt->statement.assignment.lvalue);
            LlvmValue expLoc = CompileExp(ctx, stmt->statement.assignment.rvalue,
                                             stmt->statement.assignment.lvalue->semantic.type);                                                

            LlvmEmitStore(ctx, TypeToTypeRef(ctx, stmt->statement.assignment.lvalue->semantic.type, 0), 
                          varLoc, expLoc);

            LlvmEmit(ctx, "\n");
            break;
        }

        case MON_STMT_RETURN: {
            if (stmt->statement.returnStmt.returnedExpression != NULL) {
                LlvmValue retExpLoc = CompileExp(ctx, stmt->statement.returnStmt.returnedExpression,
                                                    ctx->enclosingFunction->semantic.returnType);
                LlvmEmitRet(ctx, TypeToTypeRef(ctx, ctx->enclosingFunction->semantic.returnType, 0), retExpLoc);
            } else {
                LlvmEmitRetVoid(ctx);
            }
            return true;
        }

        case MON_STMT_CALL:
            CompileCall(ctx, stmt->statement.call);
            LlvmEmit(ctx, "\n");
            break;

        case MON_STMT_BLOCK:
            CompileBlock(ctx, stmt->statement.block);
            break;

        case MON_STMT_ECHO:
            break;

        case MON_STMT_VARDEF: {
            LlvmValue addr = CompileLocal(ctx, stmt->statement.varDef->varName, stmt->statement.varDef->semantic.type);
            LlvmEmit(ctx, "\n");
            // Store default value of variable
            LlvmEmitStore(ctx, TypeToTypeRef(ctx, stmt->statement.varDef->semantic.type, 0),
                          addr, GetDefaultValue(stmt->statement.varDef->semantic.type));
            break;
        }

        case MON_STMT_BREAK:
            LlvmEmitBranch(ctx, ctx->enclosingLoopEndLabel);
            break;

        case MON_STMT_CONTINUE:
            LlvmEmitBranch(ctx, ctx->enclosingLoopCheckLabel);
            break;           
    }

    return false;
}

static LlvmValue CompileCall(LlvmGenContext* ctx, Mon_AstCall* call) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(call);

    LlvmValue retLoc = ValLocal(-1);

    // Function might not have local implementation.
    // In this case, this call creates an external dependency to that function
    // symbol.
    AddFunctionDependency(ctx, call->semantic.callee);

    Mon_AstTypeDef* retType = call->semantic.callee->semantic.returnType;
    
    // Compile each parameter expression
    int argsCount = Mon_VectorCount(&call->parameterList);
    LlvmValue* argsBuf = NULL;
    if (argsCount > 0) {
        argsBuf = Mon_Alloc(sizeof(LlvmValue)*argsCount);    
        if (argsBuf == NULL) {
            THROW(ctx, JMP_ERRMEM);
        }

        for (int i = 0; i < argsCount; ++i) {
            Mon_AstExp* paramExp = Mon_VectorGet(&call->parameterList, i);
            Mon_AstParam* paramDef = Mon_VectorGet(&call->semantic.callee->parameters, i);
            LlvmValue loc = CompileExp(ctx, paramExp, paramDef->semantic.type);
            argsBuf[i] = loc;
        }
    }

    if (retType != BUILTIN_TABLE->types.tVoid) {
        // Function returns something, assign it to an LlvmValue.
        retLoc = ValLocal(ctx->blockCtx.nextLocalId++);
        LlvmEmit(ctx, "\t");
        LlvmEmitValue(ctx, retLoc);
        LlvmEmit(ctx, " = ");
    } else {
        LlvmEmit(ctx, "\t");
    }

    // Compile the call itself
    LlvmEmit(ctx, "call ");
    LlvmEmitTyperef(ctx, TypeToTypeRef(ctx, retType, 0));
    LlvmEmit(ctx, " @%s(", call->semantic.callee->funcName);

    for (int i = 0; i < argsCount; ++i) {
        Mon_AstParam* paramDef = Mon_VectorGet(&call->semantic.callee->parameters, i);

        LlvmEmitTyperef(ctx, TypeToTypeRef(ctx, paramDef->semantic.type, 0));
        LlvmEmit(ctx, " ");
        LlvmEmitValue(ctx, argsBuf[i]);
        if (i < argsCount - 1) {
            LlvmEmit(ctx, ", ");
        }
    }

    if (argsBuf != NULL) {
        Mon_Free(argsBuf);
    }

    LlvmEmit(ctx, ")\n");

    return retLoc;   
}

static void CompileBlock(LlvmGenContext* ctx, Mon_AstBlock* block) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(block);

    int top = Mon_VectorCount(&ctx->blockCtx.locals);

    MON_VECTOR_FOREACH(&block->statements, Mon_AstStatement*, stmt,
        if (CompileStatement(ctx, stmt)) {
            // Compiled statement always returns, simply stop compiling
            // other statements.
            break;
        }
    );

    for (int i = Mon_VectorCount(&ctx->blockCtx.locals) - 1; i >= top; --i) {
        Mon_VectorRemoveLast(&ctx->blockCtx.locals);
    }
}

static void CompileFunctionSignature(LlvmGenContext* ctx, Mon_AstFuncDef* func, bool isImplementation) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(func);

    if (isImplementation) {
        LlvmEmit(ctx, "define ");
    } else {
        LlvmEmit(ctx, "declare ");
    }

    LlvmEmitTyperef(ctx, TypeToTypeRef(ctx, func->semantic.returnType, 0));
    LlvmEmit(ctx, " @%s(", func->funcName);
    int count = Mon_VectorCount(&func->parameters);
    for (int i = 0; i < count; ++i) {
        Mon_AstParam* p = Mon_VectorGet(&func->parameters, i);
        LlvmEmitTyperef(ctx, TypeToTypeRef(ctx, p->semantic.type, 0));
                
        if (i != count - 1) {
            LlvmEmit(ctx, ", ");
        }
    }

    LlvmEmit(ctx, ")");
}

static void CompileFunctionImpl(LlvmGenContext* ctx, Mon_AstFuncDef* func) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(func);

    if (func->body == NULL) {
        return;
    }

    CompileFunctionSignature(ctx, func, true);
    
    LlvmEmit(ctx, " {\n");

    // Generate parameters local variables:
    ResetBlockContext(ctx);

    int i = 0;
    MON_VECTOR_FOREACH(&func->parameters, Mon_AstParam*, p,
        int locid = CompileLocal(ctx, p->name, p->semantic.type).locid;
        LlvmEmitStore(ctx, 
                     TypeToTypeRef(ctx, p->semantic.type, 0), 
                     ValLocal(locid), 
                     ValSSA(i++));
    );
    
    if (Mon_VectorCount(&func->parameters) > 0) {
        LlvmEmit(ctx, "\n");
    }

    if (func->semantic.isEntryPoint) {
        LlvmEmit(ctx, "\tcall void @" NAMEOF(RtInternal_Init) "()\n");
    }

    ctx->enclosingFunction = func;
    CompileBlock(ctx, func->body);
    ctx->enclosingFunction = NULL;

    if (func->semantic.returnType == BUILTIN_TABLE->types.tVoid &&
        !func->body->semantic.allPathsExit) {
        LlvmEmitRetVoid(ctx);
    }

    LlvmEmit(ctx, "}\n");
}

/** Compiles a typedef node. Returns true if a line should be skipped or not. */
static void CompileType(LlvmGenContext* ctx, Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(type);

    switch (type->typeDesc->typeDescKind) {
        case MON_TYPEDESC_ALIAS:
        case MON_TYPEDESC_ARRAY:
        case MON_TYPEDESC_SPECIAL:
        case MON_TYPEDESC_PRIMITIVE:
            return;

        case MON_TYPEDESC_RECORD:
            LlvmEmit(ctx, "%%%s = type { ", type->typeName);
            int count = Mon_VectorCount(&type->typeDesc->typeDesc.record.fields);
            for (int i = 0; i < count; ++i) {
                Mon_AstField* f = Mon_VectorGet(&type->typeDesc->typeDesc.record.fields, i);
                LlvmEmitTyperef(ctx, TypeToTypeRef(ctx, f->semantic.type, 0));
                if (i != count - 1) {
                    LlvmEmit(ctx, ", ");
                }
            }
            LlvmEmit(ctx, " }\n");
            return;
    }

    MON_UNREACHABLE();
}

static void CompileGlobalVar(LlvmGenContext* ctx, Mon_AstVarDef* def) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(def);

    LlvmEmit(ctx, "@%s = internal global ", def->varName);
    LlvmEmitTyperef(ctx, TypeToTypeRef(ctx, def->semantic.type, 0));
    LlvmEmit(ctx, " ");
    LlvmEmitValue(ctx, GetDefaultValue(def->semantic.type));
    LlvmEmit(ctx, "\n");
}

static void CompileDefinition(LlvmGenContext* ctx, Mon_AstDef* def) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(def);

    // Only used for static definitions
    switch (def->defKind) {
        case MON_AST_DEF_VAR:
            CompileGlobalVar(ctx, def->definition.variable);
            break;

        case MON_AST_DEF_FUNC:
            CompileFunctionImpl(ctx, def->definition.function);
            break;

        case MON_AST_DEF_TYPE:
            // Types are compiled on demand afterwards.
            return;
    }

    LlvmEmit(ctx, "\n");
}

static void CompileUsedTypes(LlvmGenContext* ctx) {
    MON_VECTOR_FOREACH(&ctx->targetAst->semantic.usedTypes, Mon_AstTypeDef*, type,
        CompileType(ctx, type);
        if (type->typeDesc->typeDescKind == MON_TYPEDESC_RECORD) {
            LlvmEmit(ctx, "\n");
        }
    );
}

static void GenModulePreamble(LlvmGenContext* ctx) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "source_filename = \"%s\"\n\n", ctx->targetAst->moduleName);

    LlvmEmit(ctx, "%%" TYPENAME_STRING " = type { i32, i32 }\n\n");
    LlvmEmit(ctx, "declare %%" TYPENAME_STRING "* @" NAMEOF(RtInternal_StrFromSZ) "(i8*)\n", ctx->targetAst->moduleName);

    LlvmEmit(ctx, "declare i8* @" NAMEOF(RtInternal_GcAlloc) "(i32)\n", ctx->targetAst->moduleName);
    if (ctx->targetAst->semantic.hasEntryPointFunction) {
        LlvmEmit(ctx, "declare void @" NAMEOF(RtInternal_Init) "()\n");
    }

    LlvmEmit(ctx, "\n");

    CompileUsedTypes(ctx);
}

static void CompileStringLiterals(LlvmGenContext* ctx) {
    int id = 0;

    MON_VECTOR_FOREACH(&ctx->stringLiterals, const char*, s,
        LlvmEmitString(ctx, id, s);
        id++;
    );
}

static void CompileFunctionDependencies(LlvmGenContext* ctx) {
    // Write dependencies:
    MON_VECTOR_FOREACH(&ctx->functionDependencies, Mon_AstFuncDef*, func,
        CompileFunctionSignature(ctx, func, false);
        LlvmEmit(ctx, "\n");
    );
    LlvmEmit(ctx, "\n");
    CompileStringLiterals(ctx);
    LlvmEmit(ctx, "\n");
}

static void CompileDependencies(LlvmGenContext* ctx) {
    CompileFunctionDependencies(ctx);
}

Mon_RetCode Mon_GenerateLLVM(Mon_Ast* ast,
                             FILE* outStream,
                             FILE* errStream,
                             Mon_OptLevel optizationLevel,
                             Mon_CodeGenFlags flags) {
    MON_CANT_BE_NULL(ast);
    MON_CANT_BE_NULL(outStream);
    MON_CANT_BE_NULL(errStream);

    LlvmGenContext ctx;
    jmp_buf jmpBuf;

    switch (setjmp(jmpBuf)) {
        case JMP_ERRIO:
            fprintf(ctx.errStream, "An IO error occurred during code generation.\n");
            CleanupLlvmGenContext(&ctx);
            return MON_ERR_IO;

        case JMP_ERRMEM:
            fprintf(ctx.errStream, "Code generation ran out of memory.\n");
            CleanupLlvmGenContext(&ctx);
            return MON_ERR_NOMEM;

        case JMP_SUCCESS:
            break;

        default:
            MON_ASSERT(false, "Unexpected error case.");
            CleanupLlvmGenContext(&ctx);
            return MON_ERR_NOMEM;
    }

    InitializeLlvmContext(&ctx, ast, optizationLevel, 
                          flags, outStream, errStream,
                          &jmpBuf);          

    GenModulePreamble(&ctx);      

    // Add all functions declared here to the internal functions vector.
    // This will be useful when we get to the point that we need to generate
    // dependencies.
    MON_DEFGROUP_FOREACH(&ast->definitions, def,
        if (def->defKind != MON_AST_DEF_FUNC) {
            continue;
        }

        if (def->definition.function->body == NULL) {
            // Function might have been declared here, but doesn't belong to this module.
            continue;
        }   

        RegisterInternalFunction(&ctx, def->definition.function);
    );
    // Now compile the definitions.
    MON_DEFGROUP_FOREACH(&ast->definitions, def,
        CompileDefinition(&ctx, def);
    );
    CompileDependencies(&ctx);
    CleanupLlvmGenContext(&ctx);

    return MON_SUCCESS;
}