#ifndef MON_FIELD_H
#define MON_FIELD_H

#include <mon_defines.h>

#include "definitions/mon_type_def.h"

C_LINKAGE_BEGIN

/**
 *  A record field declaration.
 */
typedef struct {

    Mon_AstNodeHeader header;

    char* typeName;
    char* fieldName;

    struct {
        Mon_AstTypeDef* type;
    } semantic;

} Mon_AstField;

/**
 *  Creates a new field declaration node.
 *
 *  @param typeName The declared field's type name.
 *  @param fieldName The declared field's name.
 * 
 *  @return The field node or NULL if allocation fails.
 * 
 *  @remarks Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstField* MON_CALL Mon_AstFieldNew(const char* typeName, const char* fieldName);

/**
 *  Destroys a field declaration node, releasing its memory.
 *  Does nothing if the specified node is NULL.
 *
 *  @param node The node to be destroyed.
 */
MON_PUBLIC void MON_CALL Mon_AstFieldDestroy(Mon_AstField* node);

C_LINKAGE_END

#endif // MON_FIELD_H