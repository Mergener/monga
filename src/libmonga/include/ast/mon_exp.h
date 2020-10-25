#ifndef MON_EXP_H
#define MON_EXP_H

#include <mon_defines.h>

#include <stdbool.h>

#include "mon_astbase.h"
#include "../mon_literal.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstExp_     Mon_AstExp;
typedef struct Mon_AstCond_    Mon_AstCond;
typedef struct Mon_AstVar_     Mon_AstVar;
typedef struct Mon_AstCall_    Mon_AstCall;
typedef struct Mon_AstTypeDef_ Mon_AstTypeDef;

typedef enum {

    MON_UNOP_NEGATIVE,  /** !operand */
    MON_UNOP_BITNOT     /** ~operand */

} Mon_UnopKind;

typedef enum {

    MON_BINOP_ADD,      /** a + b  */
    MON_BINOP_SUB,      /** a - b  */
    MON_BINOP_MUL,      /** a * b  */
    MON_BINOP_DIV,      /** a / b  */
    MON_BINOP_MODULO,   /** a % b  */
    MON_BINOP_SHR,      /** a >> b */
    MON_BINOP_SHL,      /** a << b */
    MON_BINOP_BITAND,   /** a & b  */
    MON_BINOP_BITOR,    /** a | b  */
    MON_BINOP_XOR       /** a ^ b  */

} Mon_BinopKind;

typedef enum {

    MON_EXP_CONDITIONAL, /** cond ? expA : expB */
    MON_EXP_UNOP,        /** operator operand */
    MON_EXP_BINOP,       /** expA operator expB */
    MON_EXP_CAST,        /** exp as type */
    MON_EXP_VAR,         /** A variable value access. */
    MON_EXP_LITERAL,     /** A literal constant (e.g 20) */
    MON_EXP_CALL,        /** func() */
    MON_EXP_NEW          /** new type[opt_array_exp] */

} Mon_AstExpKind;

struct Mon_AstExp_ {

    Mon_AstNodeHeader header;

    /** The type of expression stored in this node. */
    Mon_AstExpKind expKind;

    union {
        /**
         *  An expression in which value depends on the result of an operation
         *  between two operands. Available if expKind == MON_EXP_BINOP.
         */
        struct {
            Mon_AstExp* left;
            Mon_AstExp* right;
            Mon_BinopKind binOpKind;
        } binaryOperation;

        /**
         *  An expression in which value depends on the result of an operation
         *  upon a single operand. Available if expKind == MON_EXP_UNOP.
         */
        struct {
            Mon_AstExp* operand;
            Mon_UnopKind unOpKind;
        } unaryOperation;

        /**
         *  An expression with two possible values (then and else) and a condition.
         *  If the condition evaluates to true, the 'then' values is picked. Otherwise,
         *  the 'else' value is.
         */
        struct {
            Mon_AstCond* condition;
            Mon_AstExp* thenExpr;
            Mon_AstExp* elseExpr;
        } conditionalExpr;

        /**
         *  A typecast expression. Converts an inner expression to a new type.
         */
        struct {
            Mon_AstExp* castee;
            char* typeName;
        } castExpr;

        /**
         *  An instantiation expression. Allocates and constructs a new object or array.
         */
        struct {
            char* typeName;

            /** 
             *  The expression in which evaluation specifies an array size.
             *  If NULL, indicates that this new expression is not an array instantiation,
             *  but merely a single object creation.
             */
            Mon_AstExp* arraySizeExp;
        } newExpr;

        /**
         *  A variable access expression. Fetches the value from an lvalue.
         */
        Mon_AstVar* varExpr;

        /**
         *  A call expression. Its value corresponds to the value returned by the callee.
         */
        Mon_AstCall* callExpr;

        /**
         *  A literal constant provided by the source code. Can always be evaluated at compilation time.
         */
        Mon_Literal literalExpr;
    } exp;

    struct {

        Mon_AstTypeDef* type;

    } semantic;

};

/**
 *  Creates a new binary expression node. 
 *
 *  @param l The left-side expression of the node.
 *  @param r The right-side expression of the node.
 *  @param kind The desired kind of binary expression.
 * 
 *  @return The expression node or NULL if allocation fails.
 * 
 *  @remarks Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewBin(Mon_AstExp* l, Mon_AstExp* r, Mon_BinopKind kind);

/**
 *  Creates a new unary expression node. 
 *
 *  @param operand The expression's operand node.
 *  @param kind The desired kind of unary expression.
 * 
 *  @return The expression node or NULL if allocation fails.
 * 
 *  @remarks Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewUn(Mon_AstExp* operand, Mon_UnopKind kind);

/**
 *  Creates a new conditional expression node. 
 *
 *  @param cond The node relative to the condition to be evaluated.
 *  @param thenExp The node relative to the expression to be evaluated if 'cond' is true.
 *  @param elseExp The node relative to the expression to be evaluated if 'cond' is false.
 * 
 *  @return The expression node or NULL if allocation fails.
 * 
 *  @remarks Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewCond(Mon_AstCond* cond, Mon_AstExp* thenExp, Mon_AstExp* elseExp);

/**
 *  Creates a new typecast expression node. 
 *
 *  @param castee The node relative to the expression being casted.
 *  @param typeName The name of the type in which castee is being casted to.
 * 
 *  @return The expression node or NULL if allocation fails.
 * 
 *  @remarks Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewCast(Mon_AstExp* castee, const char* typeName);

/**
 *  Creates a new variable reference expression node. 
 *
 *  @param var The variable being referenced.
 * 
 *  @return The expression node or NULL if allocation fails.
 * 
 *  @remarks Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewVar(Mon_AstVar* var);

/**
 *  Creates a new call expression node.
 *
 *  @param call The call node.
 * 
 *  @return The expression node or NULL if allocation fails.
 * 
 *  @remarks Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewCall(Mon_AstCall* call);

/**
 *  Creates a new literal constant expression node.
 *
 *  @param literal The literal.
 * 
 *  @return The expression node or NULL if allocation fails.
 * 
 *  @remarks Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewLiteral(Mon_Literal literal);

/**
 *  Creates a new object/array (new operator) instantiation node.
 *
 *  @param typeName The name of the object being instantiated type.
 *  @param arraySize Expression node that will compute the instantiated array size. NULL if the 
 *  instantiation doesn't correspond to an array creation.
 * 
 *  @return The expression node or NULL if allocation fails.
 * 
 *  @remarks Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstExp* MON_CALL Mon_AstExpNewNew(const char* typeName, Mon_AstExp* arraySize);

/**
 *  Destroys an expression node, releasing its memory.
 *  Does nothing if the specified node is NULL.
 *
 *  @param node The node to be destroyed.
 *  @param rec If true, destroys all subtrees being referenced by the destroyed node.
 */
MON_PUBLIC void MON_CALL Mon_AstExpDestroy(Mon_AstExp* node, bool rec);

C_LINKAGE_END

#endif // MON_EXP_H