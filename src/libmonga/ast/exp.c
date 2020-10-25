#include "ast/mon_exp.h"

#include <mon_debug.h>
#include <string.h>

#include "mon_alloc.h"
#include "ast/mon_cond.h"
#include "ast/mon_var.h"
#include "ast/mon_call.h"
#include "../strutils.h"

static void InitializeExpSemantics(Mon_AstExp* exp) {
    exp->semantic.type = NULL;
}

Mon_AstExp* Mon_AstExpNewBin(Mon_AstExp* l, Mon_AstExp* r, Mon_BinopKind kind) {
    MON_CANT_BE_NULL(l);
    MON_CANT_BE_NULL(r);

    Mon_AstExp* ret = Mon_Alloc(sizeof(Mon_AstExp));
    if (ret == NULL) {
        return NULL;
    }

    ret->expKind = MON_EXP_BINOP;
    ret->exp.binaryOperation.left = l;
    ret->exp.binaryOperation.right = r;
    ret->exp.binaryOperation.binOpKind = kind;

    InitializeExpSemantics(ret);

    return ret;
}

Mon_AstExp* Mon_AstExpNewCall(Mon_AstCall* call) {
    MON_CANT_BE_NULL(call);

    Mon_AstExp* ret = Mon_Alloc(sizeof(Mon_AstExp));
    if (ret == NULL) {
        return NULL;
    }

    ret->expKind = MON_EXP_CALL;
    ret->exp.callExpr = call;

    InitializeExpSemantics(ret);

    return ret;
}

Mon_AstExp* Mon_AstExpNewNew(const char* typeName, Mon_AstExp* arrSizeExp) {
    MON_CANT_BE_NULL(typeName);

    Mon_AstExp* ret = Mon_Alloc(sizeof(Mon_AstExp));
    if (ret == NULL) {
        return NULL;
    }

    ret->exp.newExpr.typeName = DuplicateString(typeName, strlen(typeName));
    if (ret->exp.newExpr.typeName == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    ret->expKind = MON_EXP_NEW;
    ret->exp.newExpr.arraySizeExp = arrSizeExp;

    InitializeExpSemantics(ret);

    return ret;
}

Mon_AstExp* Mon_AstExpNewUn(Mon_AstExp* operand, Mon_UnopKind unOpKind) {
    MON_CANT_BE_NULL(operand);

    Mon_AstExp* ret = Mon_Alloc(sizeof(Mon_AstExp));
    if (ret == NULL) {
        return NULL;
    }

    ret->expKind = MON_EXP_UNOP;
    ret->exp.unaryOperation.operand = operand;
    ret->exp.unaryOperation.unOpKind = unOpKind;

    InitializeExpSemantics(ret);

    return ret;
}

Mon_AstExp* Mon_AstExpNewCond(Mon_AstCond* cond, Mon_AstExp* thenExp, Mon_AstExp* elseExp) {
    MON_CANT_BE_NULL(cond);
    MON_CANT_BE_NULL(thenExp);
    MON_CANT_BE_NULL(elseExp);

    Mon_AstExp* ret = Mon_Alloc(sizeof(Mon_AstExp));
    if (ret == NULL) {
        return NULL;
    }

    ret->expKind = MON_EXP_CONDITIONAL;
    ret->exp.conditionalExpr.condition = cond;
    ret->exp.conditionalExpr.thenExpr = thenExp;
    ret->exp.conditionalExpr.elseExpr = elseExp;

    InitializeExpSemantics(ret);

    return ret;
}

Mon_AstExp* Mon_AstExpNewCast(Mon_AstExp* castee, const char* typeName) {
    MON_CANT_BE_NULL(castee);
    MON_CANT_BE_NULL(typeName);

    Mon_AstExp* ret = Mon_Alloc(sizeof(Mon_AstExp));
    if (ret == NULL) {
        return NULL;
    }

    ret->exp.castExpr.typeName = DuplicateString(typeName, strlen(typeName));
    if (ret->exp.castExpr.typeName == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    ret->expKind = MON_EXP_CAST;
    ret->exp.castExpr.castee = castee;

    InitializeExpSemantics(ret);

    return ret;
}

Mon_AstExp* Mon_AstExpNewVar(Mon_AstVar* var) {
    MON_CANT_BE_NULL(var);

    Mon_AstExp* ret = Mon_Alloc(sizeof(Mon_AstExp));
    if (ret == NULL) {
        return NULL;
    }

    ret->expKind = MON_EXP_VAR;
    ret->exp.varExpr = var;

    InitializeExpSemantics(ret);

    return ret;
}

Mon_AstExp* Mon_AstExpNewLiteral(Mon_Literal literal) {
    Mon_AstExp* ret = Mon_Alloc(sizeof(Mon_AstExp));
    if (ret == NULL) {
        return NULL;
    }

    if (literal.literalKind == MON_LIT_STR) {
        MON_CANT_BE_NULL(literal.string.arr);

        ret->exp.literalExpr.string.arr = DuplicateString(literal.string.arr, literal.string.len);
        if (ret->exp.literalExpr.string.arr == NULL) {
            Mon_Free(ret);
            return NULL;
        }
        ret->exp.literalExpr.string.len = literal.string.len;

    } else {
        ret->exp.literalExpr = literal;
    }

    ret->expKind = MON_EXP_LITERAL;

    InitializeExpSemantics(ret);

    return ret;
}

void Mon_AstExpDestroy(Mon_AstExp* node, bool rec) {
    if (node == NULL) {
        return;
    }

    Mon_AstExp exp = *node;
    Mon_Free(node);

    switch (exp.expKind) {
        case MON_EXP_CONDITIONAL:
            if (rec) {
                Mon_AstCondDestroy(exp.exp.conditionalExpr.condition, true);
                Mon_AstExpDestroy(exp.exp.conditionalExpr.thenExpr, true);
                Mon_AstExpDestroy(exp.exp.conditionalExpr.elseExpr, true);
            }
            break;

        case MON_EXP_VAR:
            if (rec) {
                Mon_AstVarDestroy(exp.exp.varExpr, true);
            }
            break;

        case MON_EXP_UNOP:
            if (rec) {
                Mon_AstExpDestroy(exp.exp.unaryOperation.operand, true);
            }
            break;

        case MON_EXP_BINOP:
            if (rec) {
                Mon_AstExpDestroy(exp.exp.binaryOperation.left, true);
                Mon_AstExpDestroy(exp.exp.binaryOperation.right, true);
            }
            break;

        case MON_EXP_CAST:
            if (rec) {
                Mon_AstExpDestroy(exp.exp.castExpr.castee, true);
            }
            Mon_Free(exp.exp.castExpr.typeName);
            break;

        case MON_EXP_LITERAL:
            if (exp.exp.literalExpr.literalKind == MON_LIT_STR) {
                Mon_Free(exp.exp.literalExpr.string.arr);
            }
            break;

        case MON_EXP_CALL:
            if (rec) {
                Mon_AstCallDestroy(exp.exp.callExpr, true);
            }
            break;

        case MON_EXP_NEW:
            if (rec && exp.exp.newExpr.arraySizeExp != NULL) {
                Mon_AstExpDestroy(exp.exp.newExpr.arraySizeExp, true);
            }
            Mon_Free(exp.exp.newExpr.typeName);
            break;
        
        default:
            MON_ASSERT(false, "Unimplemented exp kind. (got %d)", (int)exp.expKind);
            break;
    }
}
