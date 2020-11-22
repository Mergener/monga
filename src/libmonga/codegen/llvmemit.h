#ifndef LLVMUTILS_H
#define LLVMUTILS_H

#include <mon_defines.h>

#include "../sem/types.h"

typedef struct LlvmGenContext_ LlvmGenContext;

typedef struct {

    const char* _typeName;
    int _indir;

} LlvmTypeRef;

typedef struct {

    union {
        const char* name;
        int locid;
        int localLabelId;
        int ssaId;
        Mon_Literal literal;
    };
    enum {
        LOC_LOCAL,
        LOC_LOCALLABEL,
        LOC_LITERAL,
        LOC_GLOBAL,
        LOC_SSA
    } kind;

} LlvmLocation;

typedef enum {
    
    LLVM_BINOP_ADD,
    LLVM_BINOP_SUB,
    LLVM_BINOP_SMUL,
    LLVM_BINOP_SDIV,
    LLVM_BINOP_FADD,
    LLVM_BINOP_FSUB,
    LLVM_BINOP_FMUL,
    LLVM_BINOP_FDIV,
    LLVM_BINOP_SHL,
    LLVM_BINOP_ASHR,
    LLVM_BINOP_LSHR,
    LLVM_BINOP_BITNOT,
    LLVM_BINOP_AND,
    LLVM_BINOP_OR,
    LLVM_BINOP_XOR,
    LLVM_BINOP_MOD,
    LLVM_BINOP_FREM

} LlvmBinopKind;

MON_PRIVATE LlvmTypeRef TypeToTypeRef(LlvmGenContext* ctx, const Mon_AstTypeDef* type, int indirection);

/** Creates a new LLVM global location (prefixed with @). */
MON_PRIVATE LlvmLocation LocGlobal(const char* globalName);

/** Creates a new LLVM local variable location (prefixed with %T). */
MON_PRIVATE LlvmLocation LocLocal(int locid);

MON_PRIVATE LlvmLocation LocLabel(int localLabelId);

MON_PRIVATE LlvmLocation LocSsa(int ssaId);

MON_PRIVATE LlvmLocation LocLiteral(Mon_Literal lit);

/** Writes an llvm location. */
MON_PRIVATE void LlvmEmitLocation(LlvmGenContext* ctx, LlvmLocation loc);

/** Compiles an alloca assignment. */
MON_PRIVATE void LlvmEmitAlloca(LlvmGenContext* ctx, LlvmTypeRef type, LlvmLocation loc);

/** Compiles an LLVM store statement. */
MON_PRIVATE void LlvmEmitStore(LlvmGenContext* ctx, LlvmTypeRef type, LlvmLocation destAddr, LlvmLocation src);

/** Compiles an LLVM load statement. */
MON_PRIVATE void LlvmEmitLoad(LlvmGenContext* ctx, LlvmTypeRef type, LlvmLocation srcAddr, LlvmLocation dest);

/** Writes a typename in LLVM. This automatically creates a dependency to the type. */
MON_PRIVATE void LlvmEmitTyperef(LlvmGenContext* ctx, LlvmTypeRef typeRef);

MON_PRIVATE LlvmLocation LlvmEmitFpext(LlvmGenContext* ctx, 
                                      LlvmTypeRef expType, 
                                      LlvmLocation expLoc, 
                                      LlvmTypeRef destType);

MON_PRIVATE LlvmLocation LlvmEmitFptrunc(LlvmGenContext* ctx, 
                                        LlvmTypeRef expType, 
                                        LlvmLocation expLoc, 
                                        LlvmTypeRef destType);

MON_PRIVATE LlvmLocation LlvmEmitFptosi(LlvmGenContext* ctx, 
                                       LlvmTypeRef expType, 
                                       LlvmLocation expLoc, 
                                       LlvmTypeRef destType);

MON_PRIVATE LlvmLocation LlvmEmitSitofp(LlvmGenContext* ctx, 
                                        LlvmTypeRef expType, 
                                        LlvmLocation expLoc, 
                                        LlvmTypeRef destType);

MON_PRIVATE LlvmLocation LlvmEmitSext(LlvmGenContext* ctx, 
                                      LlvmTypeRef expType, 
                                      LlvmLocation expLoc, 
                                      LlvmTypeRef destType);

MON_PRIVATE LlvmLocation LlvmEmitTrunc(LlvmGenContext* ctx, 
                                        LlvmTypeRef expType, 
                                        LlvmLocation expLoc, 
                                        LlvmTypeRef destType);

MON_PRIVATE void LlvmEmitRet(LlvmGenContext* ctx, 
                             LlvmTypeRef retType,
                             LlvmLocation retExpLoc);

MON_PRIVATE void LlvmEmitBranch(LlvmGenContext* ctx,
                                LlvmLocation targetLabelLoc);

MON_PRIVATE LlvmLocation LlvmEmitBinop(LlvmGenContext* ctx,
                                       LlvmTypeRef type,
                                       LlvmLocation aLoc,
                                       LlvmLocation bLoc,
                                       LlvmBinopKind binop);

#endif // LLVMUTILS_H