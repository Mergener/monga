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

#include "llvmctx.h"
#include "llvmemit.h"

static LlvmLocation CompileVar(LlvmGenContext* ctx, Mon_AstVar* var);
static LlvmLocation CompileCall(LlvmGenContext* ctx, Mon_AstCall* call);
static LlvmLocation CompileExp(LlvmGenContext* ctx, Mon_AstExp* exp, Mon_AstTypeDef* targetType);

/** 
 *  Compiles a local variable. The local variable data is added to the block context.
 */
static LlvmLocation CompileLocal(LlvmGenContext* ctx, const char* name, Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(name);
    MON_CANT_BE_NULL(type);

    LocalVariableData* d = AddLocal(ctx, name, type);

    LlvmEmitAlloca(ctx, TypeToTypeRef(ctx, type, 0), d->location);

    return d->location;
}

static void CompileBinCond(LlvmGenContext* ctx, Mon_BinCondKind kind, Mon_AstCond* l, Mon_AstCond* r) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(l);
    MON_CANT_BE_NULL(r);

    switch (kind) {
        case MON_BINCOND_AND:
        
            return;

        case MON_BINCOND_OR:
            return;
    }
    
    MON_UNREACHABLE();
}

static void CompileComparison(LlvmGenContext* ctx, Mon_AstCond* cond) {
    MON_CANT_BE_NULL(cond);
    MON_CANT_BE_NULL(ctx);
    MON_ASSERT(cond->condKind == MON_COND_COMPARISON,
        "Specified condition node must be a comparison. (got %d)", 
        (int)cond->condKind);
    

}

static void CompileCondition(LlvmGenContext* ctx, Mon_AstCond* cond, LlvmLocation trueLabel, LlvmLocation falseLabel) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(cond);

    switch (cond->condKind) {
        case MON_COND_BIN:

            break;

        case MON_COND_COMPARISON:
            break;
    }
}

static LlvmLocation CompileBinop(LlvmGenContext* ctx, 
                                 Mon_AstTypeDef* targetType, 
                                 Mon_AstExp* l, Mon_AstExp* r, 
                                 Mon_BinopKind binop) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(targetType);
    MON_CANT_BE_NULL(l);
    MON_CANT_BE_NULL(r);

    LlvmTypeRef targetTypeRef = TypeToTypeRef(ctx, targetType, 0);

    LlvmLocation lLoc = CompileExp(ctx, l, targetType);
    LlvmLocation rLoc = CompileExp(ctx, r, targetType);
    
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
                THROW(ctx, JMP_ERRILL);
                break;            
        }
    } else if (IsIntegerType(targetType)) {
        switch (binop) {   
            case MON_BINOP_ADD:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_ADD);

            case MON_BINOP_SUB:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_SUB);

            case MON_BINOP_MUL:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_SMUL);

            case MON_BINOP_DIV:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_SDIV);

            case MON_BINOP_MODULO:
                return LlvmEmitBinop(ctx, targetTypeRef, lLoc, rLoc, LLVM_BINOP_MOD);

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
    THROW(ctx, JMP_ERRUNK);
    return LocLocal(-1);
}

static LlvmLocation CompileFloatToFloat(LlvmGenContext* ctx,
                                        Mon_AstTypeDef* expType,
                                        LlvmLocation expLoc,
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
    THROW(ctx, JMP_ERRILL);
    return LocLocal(-1);
}

static LlvmLocation CompileIntToInt(LlvmGenContext* ctx,
                                    Mon_AstTypeDef* expType,
                                    LlvmLocation expLoc,
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

static LlvmLocation CompileConversion(LlvmGenContext* ctx, 
                                      Mon_AstTypeDef* expType, 
                                      LlvmLocation expLoc, 
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

    if (!IsTypeCastableFrom(destType, expType)) {
        THROW(ctx, JMP_ERRILL);
    }
    // We are assuming that the current rule for type casting
    // is to allow if and only if both types are primitives and
    // they aren't a 'char' + 'floatx' pair.
    // If one day we semantically add new types of conversion
    // and forget to implement them here, this assertion will
    // cause an error.
    MON_ASSERT(expType->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE &&
               destType->typeDesc->typeDescKind == MON_TYPEDESC_PRIMITIVE,
               "Conversion codegen assumes both types are primitives.");

    if (expType->typeDesc->typeDesc.primitive.typeCode == MON_PRIMITIVE_VOID ||
        destType->typeDesc->typeDesc.primitive.typeCode == MON_PRIMITIVE_VOID) {
        THROW(ctx, JMP_ERRILL);
    }

    switch (expType->typeDesc->typeDesc.primitive.typeCode) {
        case MON_PRIMITIVE_CHAR:
            MON_ASSERT(!IsFloatingPointType(destType), 
                "Conversion codegen assumes chars cannot be converted to floats.");
            break;

        case MON_PRIMITIVE_INT8:
        case MON_PRIMITIVE_INT16:
        case MON_PRIMITIVE_INT32:
        case MON_PRIMITIVE_INT64:
            MON_ASSERT(IsNumericType(destType), 
                "Conversion codegen assumes integers can only be converted to other numeric types.");
            if (IsIntegerType(destType)) {
                return CompileIntToInt(ctx, expType, expLoc, destType);
            }

            return LlvmEmitSitofp(ctx, TypeToTypeRef(ctx, expType, 0),
                                  expLoc, TypeToTypeRef(ctx, destType, 0));

        case MON_PRIMITIVE_FLOAT32:
        case MON_PRIMITIVE_FLOAT64:
            MON_ASSERT(destType->typeDesc->typeDesc.primitive.typeCode != MON_PRIMITIVE_CHAR, 
                "Conversion codegen assumes floats cannot be converted to chars.");

            if (IsFloatingPointType(destType)) {
                return CompileFloatToFloat(ctx, expType, expLoc, destType);
            }

            return LlvmEmitFptosi(ctx, TypeToTypeRef(ctx, expType, 0),
                                  expLoc, TypeToTypeRef(ctx, destType, 0));

        case MON_PRIMITIVE_VOID:
            // Unreachable, fall below.
            break;
    }

    MON_UNREACHABLE();
    THROW(ctx, JMP_ERRUNK);
    return LocLocal(-1);
}

/**
 *  Compiles an expression and returns its locid.
 */
static LlvmLocation CompileExp(LlvmGenContext* ctx, Mon_AstExp* exp, Mon_AstTypeDef* targetType) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(exp);

    LlvmLocation expLoc;

    switch (exp->expKind) {
        case MON_EXP_CONDITIONAL:
            break;

        case MON_EXP_UNOP:
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
            LlvmLocation varLoc = CompileVar(ctx, exp->exp.varExpr);
            LlvmLocation ret = LocLocal(ctx->blockCtx.nextLocalId++);
            LlvmEmitLoad(ctx, TypeToTypeRef(ctx, exp->semantic.type, 0), varLoc, ret);
            expLoc = ret;
            break;
        }

        case MON_EXP_LITERAL:
            expLoc = LocLiteral(exp->exp.literalExpr);
            break;

        case MON_EXP_CALL:
            expLoc = CompileCall(ctx, exp->exp.callExpr);            
            break;

        case MON_EXP_NEW:
            break;

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
static LlvmLocation CompileVar(LlvmGenContext* ctx, Mon_AstVar* var) {
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
            return LocGlobal(var->var.direct.name);
        }

        case MON_VAR_FIELD:
            break;

        case MON_VAR_INDEXED:
            break;
    }
}

static void CompileStatement(LlvmGenContext* ctx, Mon_AstStatement* stmt, Mon_AstFuncDef* enclosingFunction) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(stmt);

    switch (stmt->statementKind) {
        case MON_STMT_IF:
            break;

        case MON_STMT_WHILE:
            break;

        case MON_STMT_ASSIGNMENT: {
            LlvmLocation varLoc = CompileVar(ctx, stmt->statement.assignment.lvalue);
            LlvmLocation expLoc = CompileExp(ctx, stmt->statement.assignment.rvalue,
                                             stmt->statement.assignment.lvalue->semantic.type);                                                

            LlvmEmitStore(ctx, TypeToTypeRef(ctx, stmt->statement.assignment.lvalue->semantic.type, 0), 
                         varLoc, expLoc);

            LlvmEmit(ctx, "\n");
            break;
        }

        case MON_STMT_RETURN: {
            LlvmLocation retExpLoc = CompileExp(ctx, stmt->statement.returnStmt.returnedExpression,
                                                enclosingFunction->semantic.returnType);
            LlvmEmitRet(ctx, TypeToTypeRef(ctx, enclosingFunction->semantic.returnType, 0), retExpLoc);
            break;
        }

        case MON_STMT_CALL:
            CompileCall(ctx, stmt->statement.call);
            LlvmEmit(ctx, "\n");
            break;

        case MON_STMT_BLOCK:
            break;

        case MON_STMT_ECHO:
            break;

        case MON_STMT_VARDEF:
            CompileLocal(ctx, stmt->statement.varDef->varName, stmt->statement.varDef->semantic.type);
            LlvmEmit(ctx, "\n");
            break;

        case MON_STMT_BREAK:
            break;

        case MON_STMT_CONTINUE:
            break;           
    }
}

static LlvmLocation CompileCall(LlvmGenContext* ctx, Mon_AstCall* call) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(call);

    LlvmLocation retLoc = LocLocal(-1);

    // Function might not have local implementation.
    // In this case, this call creates an external dependency to that function
    // symbol.
    AddFunctionDependency(ctx, call->semantic.callee);

    Mon_AstTypeDef* retType = call->semantic.callee->semantic.returnType;
    
    // Compile each parameter expression
    int argsCount = Mon_VectorCount(&call->parameterList);
    LlvmLocation* argsBuf = NULL;
    if (argsCount > 0) {
        argsBuf = Mon_Alloc(sizeof(LlvmLocation)*argsCount);    
        if (argsBuf == NULL) {
            THROW(ctx, JMP_ERRMEM);
        }

        for (int i = 0; i < argsCount; ++i) {
            Mon_AstExp* paramExp = Mon_VectorGet(&call->parameterList, i);
            Mon_AstParam* paramDef = Mon_VectorGet(&call->semantic.callee->parameters, i);
            LlvmLocation loc = CompileExp(ctx, paramExp, paramDef->semantic.type);
            argsBuf[i] = loc;
        }
    }

    if (retType != BUILTIN_TABLE->types.tVoid) {
        // Function returns something, assign it to an LlvmLocation.
        retLoc = LocLocal(ctx->blockCtx.nextLocalId++);
        LlvmEmit(ctx, "\t");
        LlvmEmitLocation(ctx, retLoc);
        LlvmEmit(ctx, " = ");
    } else {
        LlvmEmit(ctx, "\t");
    }

    // Compile the call itself
    LlvmEmit(ctx, "call ");
    LlvmEmitTyperef(ctx, TypeToTypeRef(ctx, retType, 0));
    LlvmEmit(ctx, " @%s(", call->semantic.callee->funcName);

    for (int i = 0; i < argsCount; ++i) {
        Mon_AstExp* paramExp = Mon_VectorGet(&call->parameterList, i);
        Mon_AstParam* paramDef = Mon_VectorGet(&call->semantic.callee->parameters, i);

        LlvmEmitTyperef(ctx, TypeToTypeRef(ctx, paramDef->semantic.type, 0));
        LlvmEmit(ctx, " ");
        LlvmEmitLocation(ctx, argsBuf[i]);
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

static void CompileBlock(LlvmGenContext* ctx, Mon_AstBlock* block, Mon_AstFuncDef* enclosingFunction) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(block);

    int firstVarIdx = Mon_VectorCount(&ctx->blockCtx.locals) - 1;

    MON_VECTOR_FOREACH(&block->statements, Mon_AstStatement*, stmt,
        CompileStatement(ctx, stmt, enclosingFunction);
    );

    for (int i = Mon_VectorCount(&ctx->blockCtx.locals) - 1; i >= 0; --i) {
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
                     LocLocal(locid), 
                     LocSsa(i++));
    );
    
    if (Mon_VectorCount(&func->parameters) > 0) {
        LlvmEmit(ctx, "\n");
    }

    CompileBlock(ctx, func->body, func);

    if (func->semantic.returnType == BUILTIN_TABLE->types.tVoid) {
        LlvmEmit(ctx, "\tret void\n");
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
            return;

        case MON_TYPEDESC_RECORD:
            LlvmEmit(ctx, "%%%s = type { ", type->typeName);
            int count = Mon_VectorCount(&type->typeDesc->typeDesc.record.fields);
            int i = 0;
            for (int i = 0; i < count; ++i) {
                Mon_AstField* f = Mon_VectorGet(&type->typeDesc->typeDesc.record.fields, i);
                LlvmEmitTyperef(ctx, TypeToTypeRef(ctx, f->semantic.type, 0));
                if (i != count - 1) {
                    LlvmEmit(ctx, ", ");
                }
            }
            LlvmEmit(ctx, " }\n");
            return;

        case MON_TYPEDESC_SPECIAL:
        case MON_TYPEDESC_PRIMITIVE:
            // Ill formed, let it throw.
            break;
    }

    MON_UNREACHABLE();
    THROW(ctx, JMP_ERRILL);
}

static void CompileGlobalVar(LlvmGenContext* ctx, Mon_AstVarDef* def) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(def);

    LlvmEmit(ctx, "@%s = internal global ", def->varName);
    LlvmEmitTyperef(ctx, TypeToTypeRef(ctx, def->semantic.type, 0));
    LlvmEmit(ctx, " undef\n");
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
            break;
    }

    LlvmEmit(ctx, "\n");
}

static void GenModulePreamble(LlvmGenContext* ctx) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "source_filename = \"%s\"\n", ctx->targetAst->moduleName);

    LlvmEmit(ctx, "\n");
}

static void CompileFunctionDependencies(LlvmGenContext* ctx) {
    // Write dependencies:
    MON_VECTOR_FOREACH(&ctx->functionDependencies, Mon_AstFuncDef*, func,
        CompileFunctionSignature(ctx, func, false);
        LlvmEmit(ctx, "\n");
    );
}

static void CompileTypeDependencies(LlvmGenContext* ctx) {
    MON_VECTOR_FOREACH(&ctx->typeDependencies, Mon_AstTypeDef*, type,
        LlvmEmit(ctx, "\n");
        CompileType(ctx, type);
    );
}

static void CompileDependencies(LlvmGenContext* ctx) {
    CompileFunctionDependencies(ctx);
    CompileTypeDependencies(ctx);
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

        case JMP_ERRILL:
            fprintf(ctx.errStream, "Code generation halted; specified AST was ill-formed.\n");
            CleanupLlvmGenContext(&ctx);
            return MON_ERR_BAD_ARG;
        
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
    MON_VECTOR_FOREACH(&ast->defsVector, Mon_AstDef*, def,
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
    MON_VECTOR_FOREACH(&ast->defsVector, Mon_AstDef*, def,
        CompileDefinition(&ctx, def);
    );
    CompileDependencies(&ctx);
    CleanupLlvmGenContext(&ctx);

    return MON_SUCCESS;
}