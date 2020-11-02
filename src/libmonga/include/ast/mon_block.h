#ifndef MON_BLOCK_H
#define MON_BLOCK_H

#include <mon_defines.h>

#include "ast/mon_statement.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstVarDef_ Mon_AstVarDef;

typedef struct Mon_AstBlock_ {

    Mon_AstNodeHeader header;

    /** Stores pointers to all statements (Mon_AstVarStatement*) within this block. */
    Mon_Vector    statements;

} Mon_AstBlock;

/**
 *  Creates a new block node.
 * 
 *  @param varDefs Vector of Mon_AstVarDefs that contain all declared variables in the block.
 *  @param statements Vector of Mon_AstStatements that contain all statements in the block.
 * 
 *  @return The created block node or NULL if allocation failed.
 */
MON_PUBLIC Mon_AstBlock* MON_CALL Mon_AstBlockNew(Mon_Vector statements);

/**
 *  Destroys a block node, releasing its memory.
 * 
 *  @param node The node to be destroyed.
 *  @param rec If true, also destroys the chain of variable definition and statement nodes.
 */
MON_PUBLIC void MON_CALL Mon_AstBlockDestroy(Mon_AstBlock* node, bool rec);

C_LINKAGE_END

#endif // MON_BLOCK_H