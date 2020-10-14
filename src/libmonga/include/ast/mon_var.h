#ifndef MON_VAR_H
#define MON_VAR_H

#include <mon_defines.h>

#include "ast/mon_exp.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstVar_ Mon_AstVar;

typedef enum {

	MON_VAR_DIRECT,	/** var */
	MON_VAR_FIELD,	/** var.x */
	MON_VAR_INDEXED /** exp[expIdx] */

} Mon_AstVarKind;

/**
 * 	A var node. Represents any type of value storage that
 * 	can be placed on the left-side of an assignment.
 */
typedef struct Mon_AstVar_ {

	Mon_AstVarKind varKind;

	union {
		/** Represents an access to a field of a structured value. */
		struct {
			/** The expression that yields a structured value. */
			Mon_AstExp* expr;

			/** The yielded structured value's field to be accessed. */
			char* fieldName;
		} field;

		/** Represents an access to the element at a given index of an array. */
		struct {
			/** The array expression to fetch a value from. */
			Mon_AstExp* indexedExpr;

			/** The expression that yields the target index of the accessed array. */
			Mon_AstExp* indexExpr;
		} indexed;

		/** The variable name of a variable being directly accessed. */
		char* direct;
	} var;

} Mon_AstVar;

/**
 *	Creates an indexed variable access node.
 *
 * 	@param indexedExpr The array-yielding expression to access.
 * 	@param indexExpr The expression that provides the index to be used in the array access.
 * 
 * 	@return The variable access node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstVar* MON_CALL Mon_AstVarNewIndexed(Mon_AstExp* indexedExpr, Mon_AstExp* indexExpr);

/**
 *	Creates a field access node.
 *
 * 	@param expr The expression that provides the structure to access the field from.
 * 	@param indexExpr The name of the accessed structure field.
 * 
 * 	@return The variable access node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstVar* MON_CALL Mon_AstVarNewField(Mon_AstExp* expr, const char* fieldName);

/**
 *	Creates a direct variable access node.
 *
 * 	@param varName The name of the variable being referenced.
 * 
 * 	@return The variable access node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstVar* MON_CALL Mon_AstVarNewDirect(const char* varName);

/**
 *	Destroys a variable access node, releasing its memory.
 *	Does nothing if the specified node is NULL.
 *
 * 	@param node The node to be destroyed.
 * 	@param rec If true, destroys all subtrees being referenced by the destroyed node.
 */
MON_PUBLIC void MON_CALL Mon_AstVarDestroy(Mon_AstVar* node, bool rec);

C_LINKAGE_END

#endif // MON_VAR_H