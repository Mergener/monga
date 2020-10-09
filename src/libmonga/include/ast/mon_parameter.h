#ifndef MON_PARAMETER_H
#define MON_PARAMETER_H

#include "mon_defines.h"

#include <stdlib.h>
#include <stdbool.h>

C_LINKAGE_BEGIN

/** A parameter node. Can be chained to other parameters. */
typedef struct Mon_AstParam_ {

    char*  name;
    size_t nameLen;
    char*  typeName;
    size_t typeNameLen;

} Mon_AstParam;

MON_PUBLIC Mon_AstParam* MON_CALL Mon_AstParamNew(const char* name,
                                                  size_t nameLen,
                                                  const char* typeName,
                                                  size_t typeNameLen);

/**
 *	Destroys a parameter node, releasing its memory.
 *
 * 	@param param The node to be destroyed.
 */ 
MON_PUBLIC void MON_CALL Mon_AstParamDestroy(Mon_AstParam* param);

C_LINKAGE_END

#endif // MON_PARAMETER_H