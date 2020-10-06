#ifndef MON_RETURN_H
#define MON_RETURN_H

#include <mon_defines.h>

#include "ast/mon_exp.h"

C_LINKAGE_BEGIN

typedef struct {

	/** The returned value expression. NULL if no value is being returned. */
	Mon_AstExp* returnedExpression;

} Mon_AstReturn;

MON_PUBLIC Mon_AstReturn* MON_CALL Mon_AstReturnNew(Mon_AstExp* retExp);
MON_PUBLIC void MON_CALL Mon_AstReturnDestroy(Mon_AstReturn* node, bool rec);

C_LINKAGE_END

#endif // MON_RETURN_H