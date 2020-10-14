#ifndef MON_PARAMETER_H
#define MON_PARAMETER_H

#include "mon_defines.h"

#include <stdlib.h>
#include <stdbool.h>

C_LINKAGE_BEGIN

/** 
 *  A parameter declaration node. Not to be confused with parameters that
 *  are passed to function calls; those are expression nodes.
 */
typedef struct Mon_AstParam_ {

    char*  name;
    size_t nameLen;
    char*  typeName;
    size_t typeNameLen;

} Mon_AstParam;

/**
 *    Creates a new parameter declaration node.
 *
 *     @param name The parameter name.
 *     @param nameLen The parameter name length.
 *  @param typeName The parameter type name.
 *  @param typeNameLen The parameter type name length.
 * 
 *     @return The parameter node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstParam* MON_CALL Mon_AstParamNew(const char* name,
                                                  size_t nameLen,
                                                  const char* typeName,
                                                  size_t typeNameLen);

/**
 *    Destroys a parameter node, releasing its memory.
 *
 *     @param param The node to be destroyed.
 */ 
MON_PUBLIC void MON_CALL Mon_AstParamDestroy(Mon_AstParam* param);

C_LINKAGE_END

#endif // MON_PARAMETER_H