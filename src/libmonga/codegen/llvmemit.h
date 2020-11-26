#ifndef LLVMUTILS_H
#define LLVMUTILS_H

#include <mon_defines.h>

#include "../sem/types.h"

typedef struct LlvmGenContext_ LlvmGenContext;

typedef struct {

    const char* _typeName;
    int _indir;
    bool _builtin;

} LlvmTypeRef;

typedef struct {

    union {
        /** Available when kind == LOC_GLOBAL */
        const char* globalName;

        /** Available when kind == LOC_LOCAL */
        int locid;

        /** Available when kind == LOC_LOCALLABEL */
        int localLabelId;

        /** Available when kind == LOC_SSA */
        int ssaId;

        /** Available when kind == LOC_LITERAL */
        Mon_Literal literal;
    };
    enum {
        LOC_LOCAL,
        LOC_LOCALLABEL,
        LOC_LITERAL,
        LOC_GLOBAL,
        LOC_SSA,
        LOC_NULL
    } kind;

} LlvmValue;

typedef enum {
    
    LLVM_BINOP_ADD,
    LLVM_BINOP_SUB,
    LLVM_BINOP_MUL,
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
    LLVM_BINOP_SREM,
    LLVM_BINOP_FREM

} LlvmBinopKind;

typedef enum {

    // Signed integer comparison

    LLVM_CMP_EQ,
    LLVM_CMP_NEQ,

    LLVM_CMP_SGT,
    LLVM_CMP_SLT,
    LLVM_CMP_SGE,
    LLVM_CMP_SLE,

    // Floating point comparison

    LLVM_CMP_OEQ,
    LLVM_CMP_UNEQ,
    LLVM_CMP_OGT,
    LLVM_CMP_OLT,
    LLVM_CMP_OGE,
    LLVM_CMP_OLE,

} LlvmComparKind;

MON_PRIVATE LlvmTypeRef MakeTypeRef(const char* name, int indirection, bool builtin);

MON_PRIVATE LlvmTypeRef TypeToTypeRef(LlvmGenContext* ctx, const Mon_AstTypeDef* type, int indirection);

/** Creates a new LLVM global location (prefixed with @). */
MON_PRIVATE LlvmValue ValNamedGlobal(const char* globalName);

/** Creates a new LLVM local variable location (prefixed with %T). */
MON_PRIVATE LlvmValue ValLocal(int locid);

MON_PRIVATE LlvmValue ValLabel(int localLabelId);

MON_PRIVATE LlvmValue ValSSA(int ssaId);

MON_PRIVATE LlvmValue ValNull();

MON_PRIVATE LlvmValue ValGlobal(int globalId);

MON_PRIVATE LlvmValue ValLiteral(Mon_Literal lit);

MON_PRIVATE LlvmValue LlvmEmitBitcast(LlvmGenContext* ctx, 
                                      LlvmTypeRef srcType, 
                                      LlvmValue srcVal,
                                      LlvmTypeRef destType);

/**
 *  Begins emitting a call instruction.
 *  This function is meant to be used for calls that will get
 *  a return value. For 'statement-like' simple function calls,
 *  use LlvmBeginCallStmt.
 * 
 *  Every call to a 'BeginCall' function must be matched with
 *  and 'EndCall' afterwards.
 */
MON_PRIVATE LlvmValue LlvmBeginCallExp(LlvmGenContext* ctx,
                                       const char* funcName,
                                       LlvmTypeRef returnType);

MON_PRIVATE void LlvmBeginCallStmt(LlvmGenContext* ctx,
                                   const char* funcName,
                                   LlvmTypeRef funcReturnType);

MON_PRIVATE void LlvmCallEmitArg(LlvmGenContext* ctx,
                                 LlvmTypeRef argType,
                                 LlvmValue argValue);

MON_PRIVATE void LlvmEndCall(LlvmGenContext* ctx);

/** Writes an llvm location. */
MON_PRIVATE void LlvmEmitValue(LlvmGenContext* ctx, LlvmValue loc);

/** Compiles an alloca assignment. */
MON_PRIVATE void LlvmEmitAlloca(LlvmGenContext* ctx, LlvmTypeRef type, LlvmValue loc);

/** Compiles an LLVM store statement. */
MON_PRIVATE void LlvmEmitStore(LlvmGenContext* ctx, LlvmTypeRef type, LlvmValue destAddr, LlvmValue src);

/** Compiles an LLVM load statement. */
MON_PRIVATE void LlvmEmitLoad(LlvmGenContext* ctx, LlvmTypeRef type, LlvmValue srcAddr, LlvmValue dest);

/** Writes a typename in LLVM. This automatically creates a dependency to the type. */
MON_PRIVATE void LlvmEmitTyperef(LlvmGenContext* ctx, LlvmTypeRef typeRef);

MON_PRIVATE LlvmValue LlvmEmitFpext(LlvmGenContext* ctx, 
                                      LlvmTypeRef expType, 
                                      LlvmValue expLoc, 
                                      LlvmTypeRef destType);

MON_PRIVATE LlvmValue LlvmEmitFptrunc(LlvmGenContext* ctx, 
                                        LlvmTypeRef expType, 
                                        LlvmValue expLoc, 
                                        LlvmTypeRef destType);

MON_PRIVATE LlvmValue LlvmEmitFptosi(LlvmGenContext* ctx, 
                                       LlvmTypeRef expType, 
                                       LlvmValue expLoc, 
                                       LlvmTypeRef destType);

MON_PRIVATE LlvmValue LlvmEmitSitofp(LlvmGenContext* ctx, 
                                        LlvmTypeRef expType, 
                                        LlvmValue expLoc, 
                                        LlvmTypeRef destType);

MON_PRIVATE LlvmValue LlvmEmitSext(LlvmGenContext* ctx, 
                                      LlvmTypeRef expType, 
                                      LlvmValue expLoc, 
                                      LlvmTypeRef destType);

MON_PRIVATE LlvmValue LlvmEmitTrunc(LlvmGenContext* ctx, 
                                        LlvmTypeRef expType, 
                                        LlvmValue expLoc, 
                                        LlvmTypeRef destType);

MON_PRIVATE void LlvmEmitRet(LlvmGenContext* ctx, 
                             LlvmTypeRef retType,
                             LlvmValue retExpLoc);

MON_PRIVATE void LlvmEmitRetVoid(LlvmGenContext* ctx);

MON_PRIVATE void LlvmEmitBranch(LlvmGenContext* ctx,
                                LlvmValue targetLabelLoc);

MON_PRIVATE void LlvmEmitCondBranch(LlvmGenContext* ctx,
                                    LlvmValue boolexpr,
                                    LlvmValue trueLabel,
                                    LlvmValue falseLabel);

MON_PRIVATE LlvmValue LlvmEmitBinop(LlvmGenContext* ctx,
                                    LlvmTypeRef type,
                                    LlvmValue aLoc,
                                    LlvmValue bLoc,
                                    LlvmBinopKind binop);

MON_PRIVATE void LlvmEmitLabel(LlvmGenContext* ctx, LlvmValue label);

MON_PRIVATE LlvmValue LlvmEmitComparison(LlvmGenContext* ctx,
                                         LlvmTypeRef operandType,
                                         LlvmValue l,
                                         LlvmValue r,
                                         LlvmComparKind kind);

MON_PRIVATE LlvmValue LlvmEmitGetStructElementPtr(LlvmGenContext* ctx,
                                                  LlvmTypeRef structType,
                                                  LlvmValue structPtrVal,
                                                  int elementIdx);

MON_PRIVATE LlvmValue LlvmEmitGetArrayElementPtr(LlvmGenContext* ctx,
                                                 LlvmTypeRef type,
                                                 LlvmValue arrayPtrVal,
                                                 LlvmValue idxExpVal);

MON_PRIVATE LlvmValue LlvmEmitPhi(LlvmGenContext* ctx,
                                  LlvmTypeRef type,
                                  LlvmValue aLabel,
                                  LlvmValue aValue,
                                  LlvmValue bLabel,
                                  LlvmValue bValue);

MON_PRIVATE void LlvmEmitString(LlvmGenContext* ctx,
                                int stringId,
                                const char* str);

#endif // LLVMUTILS_H