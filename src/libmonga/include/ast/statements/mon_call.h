#ifndef MON_CALL_H
#define MON_CALL_H

#include <mon_defines.h>

#include <stdlib.h>

#include "ast/mon_exp.h"

C_LINKAGE_BEGIN

typedef struct {

	const char* funcName;
	size_t funcNameLen;
	Mon_AstExp* firstParameter;

} Mon_AstCall;

MON_PUBLIC Mon_AstCall* MON_CALL Mon_AstCallNew(const char* funcName,
                                                size_t funcNameLen,
												Mon_AstExp* firstParameter);

C_LINKAGE_END
 
#endif // MON_CALL_H