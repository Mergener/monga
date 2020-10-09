#ifndef MON_CALL_H
#define MON_CALL_H

#include <mon_defines.h>

#include <stdlib.h>
#include <stdbool.h>

#include "ast/mon_exp.h"
#include "../mon_vector.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstCall_ {

	char*       funcName;
	size_t      funcNameLen;
	Mon_Vector	parameterList;

} Mon_AstCall;

/**
 *	Creates a new call node.
 *
 * 	@param funcName The name of the function being called.
 * 	@param funcNameLen The function name length.
 * 	@param firstParameter Pointer to the first parameter being passed or NULL if none is being passed.
 * 
 * 	@return The call node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstCall* MON_CALL Mon_AstCallNew(const char* funcName,
                                                size_t funcNameLen,
												Mon_Vector parameters);

/**
 *	Destroys a call node, releasing its memory.
 *
 * 	@param node The node to be destroyed.
 * 	@param rec If true, destroys the chain of parameters starting from firstParameter as well.
 * 
 */
MON_PUBLIC void MON_CALL Mon_AstCallDestroy(Mon_AstCall* node, bool rec);

C_LINKAGE_END
 
#endif // MON_CALL_H