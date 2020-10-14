#ifndef MON_STATEMENT_H
#define MON_STATEMENT_H

#include <mon_defines.h>

#include "ast/mon_call.h"
#include "ast/mon_cond.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstVar_ Mon_AstVar;
typedef struct Mon_AstBlock_ Mon_AstBlock;

typedef enum {

    MON_STMT_IF,
    MON_STMT_WHILE,
    MON_STMT_ASSIGNMENT,
    MON_STMT_RETURN,
    MON_STMT_CALL,
    MON_STMT_BLOCK,
    MON_STMT_ECHO

} Mon_StmtKind;

typedef struct {

    Mon_AstCond*  condition;
    Mon_AstBlock* thenBlock;
    Mon_AstBlock* elseBlock;

} Mon_StmtIf;

typedef struct {

    Mon_AstCond*  condition;
    Mon_AstBlock* block;

} Mon_StmtWhile;

typedef struct {

    Mon_AstExp* returnedExpression;

} Mon_StmtReturn;

typedef struct {

    Mon_AstVar* lvalue;
    Mon_AstExp* rvalue;

} Mon_StmtAssignment;

typedef struct {

    Mon_AstExp* echoedExp;

} Mon_StmtEcho;

typedef struct Mon_AstStatement_ {

    Mon_StmtKind statementKind;
    union {

        /** Available if statementKind == MON_STMT_IF */
        Mon_StmtIf ifStmt;

        /** Available if statementKind == MON_STMT_WHILE */
        Mon_StmtWhile whileStmt;

        /** Available if statementKind == MON_STMT_RETURN */
        Mon_StmtReturn returnStmt;

        /** Available if statementKind == MON_STMT_ASSIGNMENT */
        Mon_StmtAssignment assignment;

        /** Available if statementKind == MON_STMT_CALL */
        Mon_AstCall* call;

        Mon_StmtEcho echo;

        /** Available if statementKind == MON_STMT_BLOCK */
        Mon_AstBlock* block;

    } statement;

} Mon_AstStatement;

MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewCall(Mon_AstCall* call);
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewEcho(Mon_AstExp* echoExp);
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewBlock(Mon_AstBlock* block);
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewWhile(Mon_AstCond* condition, Mon_AstBlock* block);
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewIf(Mon_AstCond* condition, Mon_AstBlock* thenBlock, Mon_AstBlock* elseBlock);
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewReturn(Mon_AstExp* returnedExp);
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewAssignment(Mon_AstVar* lvalue, Mon_AstExp* rvalue);

MON_PUBLIC void MON_CALL Mon_AstStatementDestroy(Mon_AstStatement* node, bool rec);

C_LINKAGE_END

#endif // MON_STATEMENT_H