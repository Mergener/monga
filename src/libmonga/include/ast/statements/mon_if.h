#ifndef MON_IF_H
#define MON_IF_H

#include <mon_defines.h>

#include "ast/mon_block.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstCond_ Mon_AstCond;

typedef struct {

	/** The if condition. */
	Mon_AstCond*  condition;

	/** The block that is executed if condition evaluates to true. */
	Mon_AstBlock* thenBlock;

	/** The block that is executed if condition evaluates to false. May be NULL. */
	Mon_AstBlock* elseBlock;

} Mon_AstIf;

MON_PUBLIC Mon_AstIf* MON_CALL Mon_AstIfNew(Mon_AstCond* cond,
                                            Mon_AstBlock* thenBlock,
											Mon_AstBlock* elseBlock);

MON_PUBLIC void MON_CALL Mon_AstIfDestroy(Mon_AstIf* node, bool rec);

C_LINKAGE_END

#endif // MON_IF_H