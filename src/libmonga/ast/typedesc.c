#include "ast/mon_typedesc.h"

#include <string.h>

#include "ast/mon_field.h"

#include "../strutils.h"
#include "mon_alloc.h"
#include "mon_debug.h"

Mon_AstTypeDesc* Mon_AstTypeDescNewRecord(const Mon_Vector* fields) {
    MON_CANT_BE_NULL(fields);

    Mon_AstTypeDesc* ret = Mon_Alloc(sizeof(Mon_AstTypeDesc));
    if (ret == NULL) {
        return NULL;
    }

    ret->typeDescKind = MON_TYPEDESC_RECORD;
    ret->typeDesc.record.fields = *fields;

    return ret;
}

Mon_AstTypeDesc* Mon_AstTypeDescNewArray(Mon_AstTypeDesc* innerTypeDesc) {
    MON_CANT_BE_NULL(innerTypeDesc);

    Mon_AstTypeDesc* ret = Mon_Alloc(sizeof(Mon_AstTypeDesc));
    if (ret == NULL) {
        return NULL;
    }

    ret->typeDescKind = MON_TYPEDESC_ARRAY;
    ret->typeDesc.array.innerTypeDesc = innerTypeDesc;

    return ret;
}

Mon_AstTypeDesc* Mon_AstTypeDescNewPrimitive(Mon_PrimitiveTypeCode typeCode) {
    Mon_AstTypeDesc* ret = Mon_Alloc(sizeof(Mon_AstTypeDesc));
    if (ret == NULL) {
        return NULL;
    }

    ret->typeDescKind = MON_TYPEDESC_PRIMITIVE;
    ret->typeDesc.primitive.typeCode = typeCode;

    return ret;    
}

Mon_AstTypeDesc* Mon_AstTypeDescNewAlias(const char* aliasedTypeName) {
    MON_CANT_BE_NULL(aliasedTypeName);

    Mon_AstTypeDesc* ret = Mon_Alloc(sizeof(Mon_AstTypeDesc));
    if (ret == NULL) {
        return NULL;
    }

    ret->typeDesc.alias.aliasedTypeName = DuplicateString(aliasedTypeName, strlen(aliasedTypeName));
    if (ret->typeDesc.alias.aliasedTypeName == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    ret->typeDescKind = MON_TYPEDESC_ALIAS;

    return ret;
}

void Mon_AstTypeDescDestroy(Mon_AstTypeDesc* typeDesc, bool rec) {
    if (typeDesc == NULL) {
        return;
    }

    switch (typeDesc->typeDescKind) {
        case MON_TYPEDESC_ALIAS:
            Mon_Free(typeDesc->typeDesc.alias.aliasedTypeName);
            break;

        case MON_TYPEDESC_RECORD:
            if (rec) {
                MON_VECTOR_FOREACH(&typeDesc->typeDesc.record.fields, Mon_AstField*, field,
                    Mon_AstFieldDestroy(field);
                );
            }
            Mon_VectorFinalize(&typeDesc->typeDesc.record.fields);
            break;

        case MON_TYPEDESC_ARRAY:
            if (rec) {
                Mon_AstTypeDescDestroy(typeDesc->typeDesc.array.innerTypeDesc, true);
            }
            break;

        case MON_TYPEDESC_SPECIAL:
            // Nothing else to do.
            break;
        
        default:
            MON_ASSERT(false, "Unimplemented typeDescKind. (got %d)", (int)typeDesc->typeDescKind);
            break;
    }

    Mon_Free(typeDesc);
}
