#ifndef MON_PARAMETER_H
#define MON_PARAMETER_H

#include "mon_defines.h"

#include <stdlib.h>
#include <stdbool.h>

#include "ast/mon_astbase.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstTypeDef_ Mon_AstTypeDef;

/** 
 *  A parameter declaration node. Not to be confused with parameters that
 *  are passed to function calls; those are expression nodes.
 */
typedef struct Mon_AstParam_ {

    Mon_AstNodeHeader header;

    char*  name;
    size_t nameLen;
    char*  typeName;
    size_t typeNameLen;

    struct {
        Mon_AstTypeDef* type;
    } semantic;

} Mon_AstParam;

/**
 *  Creates a new parameter declaration node.
 *
 *  @param name The parameter name.
 *  @param nameLen The parameter name length.
 *  @param typeName The parameter type name.
 *  @param typeNameLen The parameter type name length.
 * 
 *  @return The parameter node or NULL if allocation fails.
 * 
 *  @remarks Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstParam* MON_CALL Mon_AstParamNew(const char* name,
                                                  size_t nameLen,
                                                  const char* typeName,
                                                  size_t typeNameLen);

/**
 *  Destroys a parameter node, releasing its memory.
 *
 *  @param param The node to be destroyed.
 */ 
MON_PUBLIC void MON_CALL Mon_AstParamDestroy(Mon_AstParam* param);

C_LINKAGE_END

#endif // MON_PARAMETER_H