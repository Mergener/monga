#ifndef MON_EXP_H
#define MON_EXP_H

#include <mon_defines.h>

#include <stdbool.h>

#include "../mon_literal.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstExp_  Mon_AstExp;
typedef struct Mon_AstCond_ Mon_AstCond;
typedef struct Mon_AstVar_  Mon_AstVar;
typedef struct Mon_AstCall_ Mon_AstCall;

typedef enum {

    MON_UNOP_NEGATIVE,
    MON_UNOP_BITNOT

} Mon_UnopKind;

typedef enum {

    MON_BINOP_ADD,
    MON_BINOP_SUB,
    MON_BINOP_MUL,
    MON_BINOP_DIV,
    MON_BINOP_MODULO,
    MON_BINOP_SHR,
    MON_BINOP_SHL,
    MON_BINOP_BITAND,
    MON_BINOP_BITOR,
    MON_BINOP_XOR

} Mon_BinopKind;

typedef enum {

    MON_EXP_CONDITIONAL, 
    MON_EXP_UNOP,  
    MON_EXP_BINOP, 
    MON_EXP_CAST, 
    MON_EXP_VAR, 
    MON_EXP_LITERAL, 
    MON_EXP_CALL, 
    MON_EXP_NEW 

} Mon_AstExpKind;

struct Mon_AstExp_ {

    Mon_AstExpKind expKind;
    union {
        struct {
            Mon_AstExp* left;
            Mon_AstExp* right;
            Mon_BinopKind binOpKind;
        } binaryOperation;

        struct {
            Mon_AstExp* operand;
            Mon_UnopKind unOpKind;
        } unaryOperation;

        struct {
            Mon_AstCond* condition;
            Mon_AstExp* thenExpr;
            Mon_AstExp* elseExpr;
        } conditionalExpr;

        struct {
            Mon_AstExp* castee;
            char* typeName;
        } castExpr;

        struct {
            char* typeName;

            /** If NULL, this new expression is not an array instantiation. */
            Mon_AstExp* arraySizeExp;
        } newExpr;

        Mon_AstVar* varExpr;

        Mon_AstCall* callExpr;

        Mon_Literal literalExpr;
    } exp;

};

MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewBin(Mon_AstExp* l, Mon_AstExp* r, Mon_BinopKind kind);
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewUn(Mon_AstExp* operand, Mon_UnopKind unOpKind);
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewCond(Mon_AstCond* cond, Mon_AstExp* thenExp, Mon_AstExp* elseExp);
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewCast(Mon_AstExp* castee, const char* typeName);
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewVar(Mon_AstVar* var);
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewCall(Mon_AstCall* call);
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewLiteral(Mon_Literal literal);
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewNew(const char* typeName, Mon_AstExp* arraySize);
MON_PUBLIC void MON_CALL Mon_AstExpDestroy(Mon_AstExp* node, bool rec);

C_LINKAGE_END

#endif // MON_EXP_H