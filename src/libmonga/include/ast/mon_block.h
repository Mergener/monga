#ifndef MON_BLOCK_H
#define MON_BLOCK_H

#include <mon_defines.h>

#include "ast/mon_statement.h"
#include "ast/definitions/mon_var_def.h"

C_LINKAGE_BEGIN

typedef struct {

	Mon_AstVarDef*    firstVarDef;
	Mon_AstStatement* firstStatement;

} Mon_AstBlock;

/**
 *	Creates a new block node.
 * 
 * 	@param firstVarDecl Pointer to the first variable declaration node in the block.
 * 	@param firstStatement Pointer to the first statement of the block.
 * 
 * 	@return The created block node or NULL if allocation failed.
 */
MON_PUBLIC Mon_AstBlock* MON_CALL Mon_AstBlockNew(Mon_AstVarDef* firstVarDecl, Mon_AstStatement* firstStatement);

/**
 *	Destroys a block node, releasing its memory.
 * 
 * 	@param node The node to be destroyed.
 * 	@param rec If true, also destroys the chain of variable definition and statement nodes.
 */
MON_PUBLIC void MON_CALL Mon_AstBlockDestroy(Mon_AstBlock* node, bool rec);

C_LINKAGE_END

#endif // MON_BLOCK_H