#include "llvmemit.h"

#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "llvmctx.h"

#include "mon_debug.h"

LlvmTypeRef MakeTypeRef(const char* name, int indirection, bool builtin) {
    LlvmTypeRef ret;
    ret._indir = indirection;
    ret._typeName = name;
    ret._builtin = builtin;
    return ret;
}

LlvmValue ValNull() {
    LlvmValue ret;
    ret.kind = LOC_NULL;
    return ret;
}

LlvmTypeRef TypeToTypeRef(LlvmGenContext* ctx, const Mon_AstTypeDef* type, int _indir) {
    MON_CANT_BE_NULL(type);

    LlvmTypeRef ret;
    ret._indir = _indir;

    type = GetUnderlyingType(type);
    if (type->typeDesc->typeDescKind == MON_TYPEDESC_ARRAY) {
        return TypeToTypeRef(ctx, type->typeDesc->typeDesc.array.semantic.innerTypeDef, _indir + 1);
    }

    if (IsRefType(type)) {
        ret._indir++;
        ret._typeName = type->typeName;
        ret._builtin = false;
        return ret;
    }

    if (IsFloatingPointType(type)) {
        switch (Mon_GetPrimitiveSize(type->typeDesc->typeDesc.primitive.typeCode)) {
            case 2:
                ret._typeName = "half";
                break;

            case 4:
                ret._typeName = "float";
                break;

            case 8:
                ret._typeName = "double";
                break;

            case 10:
                ret._typeName = "x86_fp80";
                break;

            case 16:
                ret._typeName = "fp128";   
                break;

            default:
                MON_UNREACHABLE();
                break;         
        }
    } else {
        switch (Mon_GetPrimitiveSize(type->typeDesc->typeDesc.primitive.typeCode)) {
            case 0:
                ret._typeName = "void";
                break;

            case 1:
                ret._typeName = "i8";
                break;

            case 2:
                ret._typeName = "i16";
                break;

            case 4:
                ret._typeName = "i32";
                break;

            case 8:
                ret._typeName = "i64";   
                break;
        }
    }

    ret._builtin = true;

    return ret;
}

LlvmValue ValNamedGlobal(const char* globalName) {
    MON_CANT_BE_NULL(globalName);

    LlvmValue loc;
    loc.kind = LOC_GLOBAL;
    loc.globalName = globalName;
    return loc;
}

LlvmValue ValLocal(int locid) {
    LlvmValue loc;
    loc.kind = LOC_LOCAL;
    loc.locid = locid;
    return loc;
}

LlvmValue ValSSA(int ssaId) {
    LlvmValue loc;
    loc.kind = LOC_SSA;
    loc.ssaId = ssaId;
    return loc;
}

LlvmValue ValLabel(int localLabelId) {
    LlvmValue loc;
    loc.kind = LOC_LOCALLABEL;
    loc.localLabelId = localLabelId;
    return loc;
}

LlvmValue ValLiteral(Mon_Literal literal) {
    LlvmValue loc;
    loc.kind = LOC_LITERAL;
    loc.literal = literal;
    return loc;
}

void LlvmEmitValue(LlvmGenContext* ctx, LlvmValue loc) {
    MON_CANT_BE_NULL(ctx);

    switch (loc.kind) {        
        case LOC_LOCAL:
            LlvmEmit(ctx, "%%t.%d", loc.locid);
            return;

        case LOC_LOCALLABEL:
            LlvmEmit(ctx, "%%l.%d", loc.localLabelId);
            return;

        case LOC_GLOBAL:
            LlvmEmit(ctx, "@%s", loc.globalName);
            return;

        case LOC_SSA:
            LlvmEmit(ctx, "%%%d", loc.ssaId);
            return;

        case LOC_NULL:
            LlvmEmit(ctx, "null");
            return;

        case LOC_LITERAL:
            switch (loc.literal.literalKind) {
                case MON_LIT_STR: {
                    int id = AddOrGetStringLiteralId(ctx, loc.literal.string.arr);
                    LlvmEmit(ctx, "getelementptr inbounds ([%d x i8], [%d x i8]* @str.%d, i32 0, i32 0)", 
                        loc.literal.string.len + 1, loc.literal.string.len + 1, id);
                    break;
                }

                case MON_LIT_FLOAT:
                    LlvmEmit(ctx, "%f", loc.literal.real);
                    break;

                case MON_LIT_INT:
                    LlvmEmit(ctx, "%lld", loc.literal.integer);
                    break;
            }
            return;
    }

    MON_UNREACHABLE();
}

void LlvmEmitStore(LlvmGenContext* ctx, LlvmTypeRef type, LlvmValue destAddr, LlvmValue src) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "\tstore ");
    LlvmEmitTyperef(ctx, type);

    LlvmEmit(ctx, " ");
    LlvmEmitValue(ctx, src);
    LlvmEmit(ctx, ", ");

    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, "* ");
    LlvmEmitValue(ctx, destAddr);
    LlvmEmit(ctx, "\n");
}

void LlvmEmitLoad(LlvmGenContext* ctx, LlvmTypeRef type, LlvmValue srcAddr, LlvmValue dest) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "\t");
    LlvmEmitValue(ctx, dest);
    LlvmEmit(ctx, " = load ");
    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, ", ");
    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, "* ");
    LlvmEmitValue(ctx, srcAddr);
    LlvmEmit(ctx, "\n");
}

void LlvmEmitAlloca(LlvmGenContext* ctx, LlvmTypeRef type, LlvmValue loc) {
    MON_CANT_BE_NULL(ctx);
    
    LlvmEmit(ctx, "\t");
    LlvmEmitValue(ctx, loc);
    LlvmEmit(ctx, " = alloca ");
    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, "\n");
}

void LlvmEmitTyperef(LlvmGenContext* ctx, LlvmTypeRef typeRef) {
    MON_CANT_BE_NULL(ctx);
    
    if (!typeRef._builtin) {
        LlvmEmit(ctx, "%%");
    }

    LlvmEmit(ctx, "%s", typeRef._typeName);
    int indir = typeRef._indir;
    while (indir > 0) {
        LlvmEmit(ctx, "*");
        indir--;
    }
}

static LlvmValue CompileNumConversion(LlvmGenContext* ctx, 
                                      LlvmTypeRef expType, 
                                      LlvmValue expLoc, 
                                      LlvmTypeRef destType,
                                      const char* conversion) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(conversion);

    LlvmValue ret = ValLocal(ctx->blockCtx.nextLocalId++);

    LlvmEmit(ctx, "\t");
    LlvmEmitValue(ctx, ret);
    LlvmEmit(ctx, " = %s ", conversion);
    LlvmEmitTyperef(ctx, expType);
    LlvmEmit(ctx, " ");
    LlvmEmitValue(ctx, expLoc);
    LlvmEmit(ctx, " to ");
    LlvmEmitTyperef(ctx, destType);
    LlvmEmit(ctx, "\n");

    return ret;
}

LlvmValue LlvmEmitFpext(LlvmGenContext* ctx, 
                          LlvmTypeRef expType, 
                          LlvmValue expLoc, 
                          LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "fpext");
}

LlvmValue LlvmEmitFptrunc(LlvmGenContext* ctx, 
                            LlvmTypeRef expType, 
                            LlvmValue expLoc, 
                            LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "fptrunc");
}

LlvmValue LlvmEmitFptosi(LlvmGenContext* ctx, 
                           LlvmTypeRef expType, 
                           LlvmValue expLoc, 
                           LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "fptosi");
}

LlvmValue LlvmEmitSitofp(LlvmGenContext* ctx, 
                           LlvmTypeRef expType, 
                           LlvmValue expLoc, 
                           LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "sitofp");
}

LlvmValue LlvmEmitSext(LlvmGenContext* ctx, 
                           LlvmTypeRef expType, 
                           LlvmValue expLoc, 
                           LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "sext");
}

LlvmValue LlvmEmitTrunc(LlvmGenContext* ctx, 
                           LlvmTypeRef expType, 
                           LlvmValue expLoc, 
                           LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "trunc");
}

void LlvmEmitRet(LlvmGenContext* ctx, 
                LlvmTypeRef retType,
                LlvmValue retExpLoc) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "\tret ");
    LlvmEmitTyperef(ctx, retType);
    LlvmEmit(ctx, " ");
    LlvmEmitValue(ctx, retExpLoc);
    LlvmEmit(ctx, "\n");
}

void LlvmEmitRetVoid(LlvmGenContext* ctx) {
    MON_CANT_BE_NULL(ctx);
    LlvmEmit(ctx, "\tret void\n");
}

LlvmValue LlvmEmitBinop(LlvmGenContext* ctx,
                           LlvmTypeRef type,
                           LlvmValue aLoc,
                           LlvmValue bLoc,
                           LlvmBinopKind binop) {
    MON_CANT_BE_NULL(ctx);

    const char* opName = "";

    switch (binop) {      
        case LLVM_BINOP_ADD:
            opName = "add";
            break;

        case LLVM_BINOP_SUB:
            opName = "sub";
            break;

        case LLVM_BINOP_MUL:
            opName = "mul";
            break;

        case LLVM_BINOP_SDIV:
            opName = "sdiv";
            break;

        case LLVM_BINOP_FADD:
            opName = "fadd";
            break;

        case LLVM_BINOP_FSUB:
            opName = "fsub";
            break;

        case LLVM_BINOP_FMUL:
            opName = "fmul";
            break;

        case LLVM_BINOP_FDIV:
            opName = "fdiv";
            break;

        case LLVM_BINOP_SHL:
            opName = "shl";
            break;

        case LLVM_BINOP_ASHR:
            opName = "ashr";
            break;

        case LLVM_BINOP_LSHR:
            opName = "lshr";
            break;

        case LLVM_BINOP_BITNOT:
            opName = "not";
            break;

        case LLVM_BINOP_AND:
            opName = "and";
            break;

        case LLVM_BINOP_OR:
            opName = "or";
            break;

        case LLVM_BINOP_XOR:
            opName = "xor";
            break;

        case LLVM_BINOP_SREM:
            opName = "srem";
            break;

        case LLVM_BINOP_FREM:
            opName = "frem";
            break;
    }

    LlvmValue retLoc = ValLocal(ctx->blockCtx.nextLocalId++);
    LlvmEmit(ctx, "\t");
    LlvmEmitValue(ctx, retLoc);
    LlvmEmit(ctx, " = %s ", opName);
    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, " ");
    LlvmEmitValue(ctx, aLoc);
    LlvmEmit(ctx, ", ");
    LlvmEmitValue(ctx, bLoc);
    LlvmEmit(ctx, "\n");

    return retLoc;
}

void LlvmEmitLabel(LlvmGenContext* ctx, LlvmValue label) {
    MON_CANT_BE_NULL(ctx);

    MON_ASSERT(label.kind == LOC_LOCALLABEL, 
        "'label' argument of LlvmEmitLabel must be of kind LOCALLABEL.");

    LlvmEmit(ctx, "l.%d:\n", label.localLabelId);
}

void LlvmEmitBranch(LlvmGenContext* ctx,
                    LlvmValue targetLabelLoc) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "\tbr label ");
    LlvmEmitValue(ctx, targetLabelLoc);
    LlvmEmit(ctx, "\n");
}

void LlvmEmitCondBranch(LlvmGenContext* ctx,
                        LlvmValue boolexpr,
                        LlvmValue trueLabel,
                        LlvmValue falseLabel) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "\tbr i1 ");
    LlvmEmitValue(ctx, boolexpr);
    LlvmEmit(ctx, ", label ");
    LlvmEmitValue(ctx, trueLabel);
    LlvmEmit(ctx, ", label ");
    LlvmEmitValue(ctx, falseLabel);
    LlvmEmit(ctx, "\n");
}

LlvmValue LlvmEmitComparison(LlvmGenContext* ctx,
                             LlvmTypeRef operandType,
                             LlvmValue l,
                             LlvmValue r,
                             LlvmComparKind kind) {
    MON_CANT_BE_NULL(ctx);

    const char* cmpStr = NULL; // integer/float/pointer/etc comparison
    const char* opStr = NULL; // comparison operator

    switch (kind) {
        case LLVM_CMP_EQ:
            cmpStr = "icmp";
            opStr = "eq";
            break;

        case LLVM_CMP_NEQ:
            cmpStr = "icmp";
            opStr = "ne";
            break;

        case LLVM_CMP_SGT:
            cmpStr = "icmp";
            opStr = "sgt";
            break;

        case LLVM_CMP_SLT:
            cmpStr = "icmp";
            opStr = "slt";
            break;

        case LLVM_CMP_SGE:
            cmpStr = "icmp";
            opStr = "sge";
            break;

        case LLVM_CMP_SLE:
            cmpStr = "icmp";
            opStr = "sle";
            break;

        case LLVM_CMP_OEQ:
            cmpStr = "fcmp";
            opStr = "oeq";
            break;

        case LLVM_CMP_UNEQ:
            cmpStr = "fcmp";
            opStr = "une";
            break;

        case LLVM_CMP_OGT:
            cmpStr = "fcmp";
            opStr = "ogt";
            break;

        case LLVM_CMP_OLT:
            cmpStr = "fcmp";
            opStr = "olt";
            break;

        case LLVM_CMP_OGE:
            cmpStr = "fcmp";
            opStr = "oge";
            break;

        case LLVM_CMP_OLE:
            cmpStr = "fcmp";
            opStr = "ole";
            break;
    }

    MON_CANT_BE_NULL(cmpStr);
    MON_CANT_BE_NULL(opStr);

    LlvmValue boolexpr = ValLocal(ctx->blockCtx.nextLocalId++);

    LlvmEmit(ctx, "\t");
    LlvmEmitValue(ctx, boolexpr);
    LlvmEmit(ctx, " = %s %s ", cmpStr, opStr);
    LlvmEmitTyperef(ctx, operandType);
    LlvmEmit(ctx, " ");
    LlvmEmitValue(ctx, l);
    LlvmEmit(ctx, ", ");
    LlvmEmitValue(ctx, r);
    LlvmEmit(ctx, "\n");

    return boolexpr;
}

LlvmValue LlvmEmitGetStructElementPtr(LlvmGenContext* ctx,
                                      LlvmTypeRef structType,
                                      LlvmValue structPtrVal,
                                      int elementIdx) {
    MON_CANT_BE_NULL(ctx);

    LlvmValue ret = ValLocal(ctx->blockCtx.nextLocalId++);

    LlvmEmit(ctx, "\t");
    LlvmEmitValue(ctx, ret);
    LlvmEmit(ctx, " = getelementptr inbounds ");
    structType._indir--;
    LlvmEmitTyperef(ctx, structType);
    structType._indir++;
    LlvmEmit(ctx, ", ");
    LlvmEmitTyperef(ctx, structType);
    LlvmEmit(ctx, " ");
    LlvmEmitValue(ctx, structPtrVal);
    LlvmEmit(ctx, ", i32 0, i32 %d\n", elementIdx);  

    return ret;  
}

LlvmValue LlvmEmitGetArrayElementPtr(LlvmGenContext* ctx,
                                     LlvmTypeRef type,
                                     LlvmValue arrayPtrVal,
                                     LlvmValue idxExpVal) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "\t");
    LlvmValue ret = ValLocal(ctx->blockCtx.nextLocalId++);
    LlvmEmitValue(ctx, ret);
    LlvmEmit(ctx, " = getelementptr ");
    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, ", ");
    type._indir++;
    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, " ");
    LlvmEmitValue(ctx, arrayPtrVal);
    LlvmEmit(ctx, ", i64 ");
    LlvmEmitValue(ctx, idxExpVal);
    LlvmEmit(ctx, "\n");
    return ret;
}

LlvmValue LlvmEmitPhi(LlvmGenContext* ctx,
                      LlvmTypeRef type,
                      LlvmValue aLabel,
                      LlvmValue aValue,
                      LlvmValue bLabel,
                      LlvmValue bValue) {
    MON_CANT_BE_NULL(ctx);

    LlvmValue ret = ValLocal(ctx->blockCtx.nextLocalId++);
    LlvmEmit(ctx, "\t");
    LlvmEmitValue(ctx, ret);
    LlvmEmit(ctx, " = phi ");
    LlvmEmitTyperef(ctx, type);

    LlvmEmit(ctx, " [");
    LlvmEmitValue(ctx, aValue);
    LlvmEmit(ctx, ", ");
    LlvmEmitValue(ctx, aLabel);

    LlvmEmit(ctx, "], [");
    LlvmEmitValue(ctx, bValue);
    LlvmEmit(ctx, ", ");
    LlvmEmitValue(ctx, bLabel);
    LlvmEmit(ctx, "]\n");

    return ret;
}

void LlvmEmitString(LlvmGenContext* ctx, int id, const char* s) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(s);

    LlvmEmit(ctx, "@str.%d = private unnamed_addr constant [%d x i8] c\"", 
             id, strlen(s) + 1);

    while (*s != '\0') {
        if (!iscntrl(*s)) {
            LlvmEmit(ctx, "%c", *s);
        } else {
            LlvmEmit(ctx, "\\%02X", (int)(*s));
        }
        s++;
    }

    LlvmEmit(ctx, "\\00\", align 1\n");
}