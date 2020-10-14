#ifndef MON_STATEMENT_H
#define MON_STATEMENT_H

#include <mon_defines.h>

#include "ast/mon_call.h"
#include "ast/mon_cond.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstVar_ Mon_AstVar;
typedef struct Mon_AstBlock_ Mon_AstBlock;

typedef enum {

    MON_STMT_IF,         /* if (cond) thenblock opt_elseblock */   
    MON_STMT_WHILE,      /* while (cond) doblock */   
    MON_STMT_ASSIGNMENT, /* var = exp; */
    MON_STMT_RETURN,     /* return; OR return exp; */
    MON_STMT_CALL,       /* func() */   
    MON_STMT_BLOCK,      /* { statements } */   
    MON_STMT_ECHO        /* @exp */   

} Mon_StmtKind;

/**
 *  An if statement. Contains a condition and two blocks (then and else).
 *  If the condition evaluates to true, the then block is executed.
 *  Otherwise, the else, if available, is.
 *  The else block is optional.
 */
typedef struct {

    /** The 'if' condition. */
    Mon_AstCond*  condition;

    /** The 'then' block. */
    Mon_AstBlock* thenBlock;

    /** 
     *  The 'else' block. May be NULL if nothing should 
     *  be executed in case condition evaluates to false. 
     */
    Mon_AstBlock* elseBlock;

} Mon_StmtIf;

/**
 *  A while statement. Executes its internal execution block until
 *  its condition evaluates to false.
 */
typedef struct {

    /** The 'while' condition. */
    Mon_AstCond*  condition;

    /** The execution block. */
    Mon_AstBlock* block;

} Mon_StmtWhile;

/** 
 *  A return statement. Leaves the current function's execution block, while
 *  returning to the previous function's execution. Optionally contains a value
 *  that can be retrieved by the caller function.
 */
typedef struct {

    /** The expression being returned. May be NULL if no value is to be returned. */
    Mon_AstExp* returnedExpression;

} Mon_StmtReturn;

/**
 *  An assignment. Copies the value from an expression to a storage.
 */
typedef struct {

    /**
     *  The left-side value of the assignment (storage to copy 'rvalue' to).
     */
    Mon_AstVar* lvalue;

    /**
     *  The right-side value of the assignment (the value being copied).
     */
    Mon_AstExp* rvalue;

} Mon_StmtAssignment;

/**
 *  An echo statement. Outputs an expression to the program's stdout.
 */
typedef struct {

    Mon_AstExp* echoedExp;

} Mon_StmtEcho;

/**
 *  A statement node. A statement is any concrete action that is executed
 *  within a block of execution.
 */
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

        /** Available if statementKind == MON_STMT_ECHO */
        Mon_StmtEcho echo;

        /** Available if statementKind == MON_STMT_BLOCK */
        Mon_AstBlock* block;

    } statement;

} Mon_AstStatement;

/**
 *    Creates a new call statement node.
 *
 *     @param calL The call node being referred to.
 * 
 *     @return The statement node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewCall(Mon_AstCall* call);

/**
 *    Creates a new echo statement node.
 *
 *     @param echoExp The expression to be echoed.
 * 
 *     @return The statement node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewEcho(Mon_AstExp* echoExp);

/**
 *    Creates a new execution block statement.
 *
 *     @param block The execution block.
 * 
 *     @return The statement node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewBlock(Mon_AstBlock* block);

/**
 *    Creates a new while statement.
 *
 *     @param condition The 'while' condition.
 *     @param block The 'while' execution block.
 * 
 *     @return The statement node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewWhile(Mon_AstCond* condition, Mon_AstBlock* block);

/**
 *    Creates a new if statement.
 *
 *     @param condition The 'if' condition.
 *     @param thenBlock The 'then' execution block.
 *     @param elseBlock The 'else' execution block.
 * 
 *     @return The statement node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewIf(Mon_AstCond* condition, Mon_AstBlock* thenBlock, Mon_AstBlock* elseBlock);

/**
 *    Creates a new return statement.
 *
 *     @param returnedExp The expression returned by the 'return'. May be NULL if no expression
 *  is to be returned.
 * 
 *     @return The statement node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewReturn(Mon_AstExp* returnedExp);

/**
 *    Creates a new assignment statement.
 *
 *     @param lvalue The left-side of the assignment.
 *  @param rvalue The right-side of the assignment.
 * 
 *     @return The statement node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstStatement* MON_CALL Mon_AstStatementNewAssignment(Mon_AstVar* lvalue, Mon_AstExp* rvalue);

/**
 *    Destroys a statement node, releasing its memory.
 *    Does nothing if the specified node is NULL.
 *
 *     @param node The node to be destroyed.
 *     @param rec If true, destroys all subtrees being referenced by the node.
 */
MON_PUBLIC void MON_CALL Mon_AstStatementDestroy(Mon_AstStatement* node, bool rec);

C_LINKAGE_END

#endif // MON_STATEMENT_H