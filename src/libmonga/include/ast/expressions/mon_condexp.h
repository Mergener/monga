#ifndef MON_CONDEXP_H
#define MON_CONDEXP_H

#include <mon_defines.h>

#include "ast/mon_exp.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstCond_ Mon_AstCond;

typedef struct {

	Mon_AstCond* condition;
	Mon_AstExp* thenExp;
	Mon_AstExp* elseExp;

} Mon_AstCondExp;

MON_PUBLIC Mon_AstCondExp* MON_CALL Mon_AstCondExpNew(Mon_AstCond* condition, Mon_AstExp* thenExp, Mon_AstExp* elseExp);
MON_PUBLIC void MON_CALL Mon_AstCondExpDestroy(Mon_AstCondExp* node, bool rec);

C_LINKAGE_END

#endif // MON_CONDEXP_H