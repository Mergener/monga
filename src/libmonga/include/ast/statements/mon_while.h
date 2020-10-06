#ifndef MON_WHILE_H
#define MON_WHILE_H

#include <mon_defines.h>

#include "ast/mon_block.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstCond_ Mon_AstCond;

typedef struct {

	Mon_AstCond*  condition;
	Mon_AstBlock* block;

} Mon_AstWhile;

MON_PUBLIC Mon_AstWhile* MON_CALL Mon_AstWhileNew(Mon_AstCond* cond, Mon_AstBlock* block);
MON_PUBLIC void MON_CALL Mon_AstWhileDestroy(Mon_AstWhile* node, bool rec);

C_LINKAGE_END

#endif // MON_WHILE_H