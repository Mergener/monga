#ifndef MON_STATEMENT_H
#define MON_STATEMENT_H

#include <mon_defines.h>

#include "ast/statements/mon_call.h"
#include "ast/statements/mon_assignment.h"
#include "ast/statements/mon_if.h"
#include "ast/statements/mon_return.h"
#include "ast/statements/mon_while.h"
#include "ast/mon_block.h"

C_LINKAGE_BEGIN

typedef enum {

    MON_STMT_IF,
    MON_STMT_WHILE,
    MON_STMT_ASSIGNMENT,
    MON_STMT_RETURN,
    MON_STMT_CALL,
    MON_STMT_BLOCK

} Mon_StmtKind;

typedef struct Mon_AstStatement_ {

    union {

        /** Available if statementKind == MON_STMT_IF */
        Mon_AstIf*	   ifBlock;

        /** Available if statementKind == MON_STMT_WHILE */
        Mon_AstWhile*  whileBlock;

        /** Available if statementKind == MON_STMT_RETURN */
        Mon_AstReturn* returnStmt;

        /** Available if statementKind == MON_STMT_CALL */
        Mon_AstCall*   call;

        /** Available if statementKind == MON_STMT_BLOCK */
        Mon_AstBlock*  block;

    } statement;

    Mon_StmtKind statementKind;

    struct Mon_AstStatement_* next;

} Mon_AstStatement;

MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewCall(Mon_AstCall* call);
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewBlock(Mon_AstBlock* block);
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewWhile(Mon_AstWhile* whileBlock);
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewIf(Mon_AstIf* ifBlock);
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewReturn(Mon_AstReturn* retStmt);

MON_PUBLIC void MON_CALL Mon_AstStatementDestroy(Mon_AstStatement* node, bool rec);

C_LINKAGE_END

#endif // MON_STATEMENT_H