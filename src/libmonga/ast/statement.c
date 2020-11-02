#include "ast/mon_statement.h"

#include <mon_debug.h>

#include "mon_alloc.h"
#include "ast/mon_var.h"
#include "ast/mon_block.h"
#include "ast/definitions/mon_var_def.h"

Mon_AstStatement* Mon_AstStatementNewCall(Mon_AstCall* call) {
    MON_CANT_BE_NULL(call);

    Mon_AstStatement* ret = Mon_Alloc(sizeof(Mon_AstStatement));
    if (ret == NULL) {
        return NULL;
    }

    ret->statementKind = MON_STMT_CALL;
    ret->statement.call = call;

    return ret;
}

Mon_AstStatement* Mon_AstStatementNewEcho(Mon_AstExp* echoExp) {
    MON_CANT_BE_NULL(echoExp);

    Mon_AstStatement* ret = Mon_Alloc(sizeof(Mon_AstStatement));
    if (ret == NULL) {
        return NULL;
    }

    ret->statementKind = MON_STMT_ECHO;
    ret->statement.echo.echoedExp = echoExp;

    return ret;
}

Mon_AstStatement* Mon_AstStatementNewBlock(Mon_AstBlock* block) {
    MON_CANT_BE_NULL(block);

    Mon_AstStatement* ret = Mon_Alloc(sizeof(Mon_AstStatement));
    if (ret == NULL) {
        return NULL;
    }

    ret->statementKind = MON_STMT_BLOCK;
    ret->statement.block = block;

    return ret;
}

Mon_AstStatement* Mon_AstStatementNewWhile(Mon_AstCond* condition, Mon_AstBlock* block) {
    MON_CANT_BE_NULL(condition);

    Mon_AstStatement* ret = Mon_Alloc(sizeof(Mon_AstStatement));
    if (ret == NULL) {
        return NULL;
    }

    ret->statementKind = MON_STMT_WHILE;
    ret->statement.whileStmt.condition = condition;
    ret->statement.whileStmt.block = block;

    return ret;
}

Mon_AstStatement* Mon_AstStatementNewIf(Mon_AstCond* condition, Mon_AstBlock* thenBlock, Mon_AstBlock* elseBlock) {
    MON_CANT_BE_NULL(condition);

    Mon_AstStatement* ret = Mon_Alloc(sizeof(Mon_AstStatement));
    if (ret == NULL) {
        return NULL;
    }

    ret->statementKind = MON_STMT_IF;
    ret->statement.ifStmt.condition = condition;
    ret->statement.ifStmt.thenBlock = thenBlock;
    ret->statement.ifStmt.elseBlock = elseBlock;

    return ret;
}

Mon_AstStatement* Mon_AstStatementNewReturn(Mon_AstExp* retExp) {
    Mon_AstStatement* ret = Mon_Alloc(sizeof(Mon_AstStatement));
    if (ret == NULL) {
        return NULL;
    }

    ret->statementKind = MON_STMT_RETURN;
    ret->statement.returnStmt.returnedExpression = retExp;

    return ret;
}

MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewVarDef(Mon_AstVarDef* varDef) {
    MON_CANT_BE_NULL(varDef);

    Mon_AstStatement* ret = Mon_Alloc(sizeof(Mon_AstStatement));
    if (ret == NULL) {
        return NULL;
    }

    ret->statementKind = MON_STMT_VARDEF;
    ret->statement.varDef = varDef;

    return ret;
}

Mon_AstStatement* Mon_AstStatementNewAssignment(Mon_AstVar* lvalue, Mon_AstExp* rvalue) {
    MON_CANT_BE_NULL(lvalue);
    MON_CANT_BE_NULL(rvalue);

    Mon_AstStatement* ret = Mon_Alloc(sizeof(Mon_AstStatement));
    if (ret == NULL) {
        return NULL;
    }

    ret->statementKind = MON_STMT_ASSIGNMENT;
    ret->statement.assignment.lvalue = lvalue;
    ret->statement.assignment.rvalue = rvalue;

    return ret;
}

void Mon_AstStatementDestroy(Mon_AstStatement* node, bool rec) {
    if (node == NULL) {
        return;
    }

    if (rec) {
        switch (node->statementKind) {
            case MON_STMT_IF:
                Mon_AstCondDestroy(node->statement.ifStmt.condition, true);
                Mon_AstBlockDestroy(node->statement.ifStmt.thenBlock, true);
                Mon_AstBlockDestroy(node->statement.ifStmt.elseBlock, true);
                break;

            case MON_STMT_WHILE:
                Mon_AstCondDestroy(node->statement.whileStmt.condition, true);
                Mon_AstBlockDestroy(node->statement.block, true);
                break;

            case MON_STMT_ASSIGNMENT:
                Mon_AstVarDestroy(node->statement.assignment.lvalue, true);
                Mon_AstExpDestroy(node->statement.assignment.rvalue, true);
                break;

            case MON_STMT_RETURN:
                Mon_AstExpDestroy(node->statement.returnStmt.returnedExpression, true);
                break;

            case MON_STMT_CALL:
                Mon_AstCallDestroy(node->statement.call, true);
                break;

            case MON_STMT_BLOCK:
                Mon_AstBlockDestroy(node->statement.block, true);
                break;

            case MON_STMT_ECHO:
                Mon_AstExpDestroy(node->statement.echo.echoedExp, true);
                break;

            case MON_STMT_VARDEF:
                Mon_AstVarDefDestroy(node->statement.varDef);
                break;

            default:
                MON_ASSERT(false, "Unimplemented statement kind. (got %d)", (int)node->statementKind);
                break;
        }
    }

    Mon_Free(node);
}