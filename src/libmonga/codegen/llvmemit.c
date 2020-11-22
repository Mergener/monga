#include "llvmemit.h"

#include <stdint.h>

#include "llvmctx.h"

#include "mon_debug.h"

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
        return ret;
    }

    switch (type->typeDesc->typeDesc.primitive.typeCode) {
        case MON_PRIMITIVE_CHAR:
        case MON_PRIMITIVE_INT8:
            ret._typeName = "i8";
            break;

        case MON_PRIMITIVE_INT16:
            ret._typeName = "i16";
            break;

        case MON_PRIMITIVE_INT32:
            ret._typeName = "i32";
            break;

        case MON_PRIMITIVE_INT64:
            ret._typeName = "i64";
            break;

        case MON_PRIMITIVE_FLOAT32:
            ret._typeName = "float";
            break;

        case MON_PRIMITIVE_FLOAT64:
            ret._typeName = "double";
            break;

        case MON_PRIMITIVE_VOID:
            ret._typeName = "void";
            break;
    }

    return ret;
}

LlvmLocation LocGlobal(const char* globalName) {
    MON_CANT_BE_NULL(globalName);

    LlvmLocation loc;
    loc.kind = LOC_GLOBAL;
    loc.name = globalName;
    return loc;
}

LlvmLocation LocLocal(int locid) {
    LlvmLocation loc;
    loc.kind = LOC_LOCAL;
    loc.locid = locid;
    return loc;
}

LlvmLocation LocSsa(int ssaId) {
    LlvmLocation loc;
    loc.kind = LOC_SSA;
    loc.ssaId = ssaId;
    return loc;
}

LlvmLocation LocLabel(int localLabelId) {
    LlvmLocation loc;
    loc.kind = LOC_LOCALLABEL;
    loc.localLabelId = localLabelId;
    return loc;
}

LlvmLocation LocLiteral(Mon_Literal literal) {
    LlvmLocation loc;
    loc.kind = LOC_LITERAL;
    loc.literal = literal;
    return loc;
}

void LlvmEmitLocation(LlvmGenContext* ctx, LlvmLocation loc) {
    MON_CANT_BE_NULL(ctx);

    switch (loc.kind) {        
        case LOC_LOCAL:
            LlvmEmit(ctx, "%%t.%d", loc.locid);
            return;

        case LOC_LOCALLABEL:
            LlvmEmit(ctx, "%%l.%d", loc.localLabelId);
            return;

        case LOC_GLOBAL:
            LlvmEmit(ctx, "@%s", loc.name);
            return;

        case LOC_SSA:
            LlvmEmit(ctx, "%%%d", loc.ssaId);
            return;

        case LOC_LITERAL:
            switch (loc.literal.literalKind) {
                case MON_LIT_STR:

                    break;

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

void LlvmEmitStore(LlvmGenContext* ctx, LlvmTypeRef type, LlvmLocation destAddr, LlvmLocation src) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "\tstore ");
    LlvmEmitTyperef(ctx, type);

    LlvmEmit(ctx, " ");
    LlvmEmitLocation(ctx, src);
    LlvmEmit(ctx, ", ");
    
    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, "* ");
    LlvmEmitLocation(ctx, destAddr);
    LlvmEmit(ctx, "\n");
}

void LlvmEmitLoad(LlvmGenContext* ctx, LlvmTypeRef type, LlvmLocation srcAddr, LlvmLocation dest) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "\t");
    LlvmEmitLocation(ctx, dest);
    LlvmEmit(ctx, " = load ");
    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, ", ");
    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, "* ");
    LlvmEmitLocation(ctx, srcAddr);
    LlvmEmit(ctx, "\n");
}

void LlvmEmitAlloca(LlvmGenContext* ctx, LlvmTypeRef type, LlvmLocation loc) {
    MON_CANT_BE_NULL(ctx);
    
    LlvmEmit(ctx, "\t");
    LlvmEmitLocation(ctx, loc);
    LlvmEmit(ctx, " = alloca ");
    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, "\n");
}

void LlvmEmitTyperef(LlvmGenContext* ctx, LlvmTypeRef typeRef) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "%s", typeRef._typeName);
    int indir = typeRef._indir;
    while (indir > 0) {
        LlvmEmit(ctx, "*");
        indir--;
    }
}

static LlvmLocation CompileNumConversion(LlvmGenContext* ctx, 
                                        LlvmTypeRef expType, 
                                        LlvmLocation expLoc, 
                                        LlvmTypeRef destType,
                                        const char* conversion) {
    MON_CANT_BE_NULL(ctx);
    MON_CANT_BE_NULL(conversion);

    LlvmLocation ret = LocLocal(ctx->blockCtx.nextLocalId++);

    LlvmEmit(ctx, "\t");
    LlvmEmitLocation(ctx, ret);
    LlvmEmit(ctx, " = %s ", conversion);
    LlvmEmitTyperef(ctx, expType);
    LlvmEmit(ctx, " ");
    LlvmEmitLocation(ctx, expLoc);
    LlvmEmit(ctx, " to ");
    LlvmEmitTyperef(ctx, destType);
    LlvmEmit(ctx, "\n");

    return ret;
}

LlvmLocation LlvmEmitFpext(LlvmGenContext* ctx, 
                          LlvmTypeRef expType, 
                          LlvmLocation expLoc, 
                          LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "fpext");
}

LlvmLocation LlvmEmitFptrunc(LlvmGenContext* ctx, 
                            LlvmTypeRef expType, 
                            LlvmLocation expLoc, 
                            LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "fptrunc");
}

LlvmLocation LlvmEmitFptosi(LlvmGenContext* ctx, 
                           LlvmTypeRef expType, 
                           LlvmLocation expLoc, 
                           LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "fptosi");
}

LlvmLocation LlvmEmitSitofp(LlvmGenContext* ctx, 
                           LlvmTypeRef expType, 
                           LlvmLocation expLoc, 
                           LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "sitofp");
}

LlvmLocation LlvmEmitSext(LlvmGenContext* ctx, 
                           LlvmTypeRef expType, 
                           LlvmLocation expLoc, 
                           LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "sext");
}

LlvmLocation LlvmEmitTrunc(LlvmGenContext* ctx, 
                           LlvmTypeRef expType, 
                           LlvmLocation expLoc, 
                           LlvmTypeRef destType) {
    MON_CANT_BE_NULL(ctx);

    return CompileNumConversion(ctx, expType, expLoc, destType, "trunc");
}

void LlvmEmitRet(LlvmGenContext* ctx, 
                LlvmTypeRef retType,
                LlvmLocation retExpLoc) {
    MON_CANT_BE_NULL(ctx);

    LlvmEmit(ctx, "\tret ");
    LlvmEmitTyperef(ctx, retType);
    LlvmEmit(ctx, " ");
    LlvmEmitLocation(ctx, retExpLoc);
    LlvmEmit(ctx, "\n");
}

LlvmLocation LlvmEmitBinop(LlvmGenContext* ctx,
                           LlvmTypeRef type,
                           LlvmLocation aLoc,
                           LlvmLocation bLoc,
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

        case LLVM_BINOP_SMUL:
            opName = "smul";
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

        case LLVM_BINOP_MOD:
            opName = "mod";
            break;

        case LLVM_BINOP_FREM:
            opName = "frem";
            break;
    }

    LlvmLocation retLoc = LocLocal(ctx->blockCtx.nextLocalId++);
    LlvmEmit(ctx, "\t");
    LlvmEmitLocation(ctx, retLoc);
    LlvmEmit(ctx, " = %s ", opName);
    LlvmEmitTyperef(ctx, type);
    LlvmEmit(ctx, " ");
    LlvmEmitLocation(ctx, aLoc);
    LlvmEmit(ctx, ", ");
    LlvmEmitLocation(ctx, bLoc);
    LlvmEmit(ctx, "\n");

    return retLoc;
}

