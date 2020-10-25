#include "ast/mon_field.h"

#include <string.h>

#include <mon_debug.h>

#include "mon_alloc.h"
#include "../strutils.h"

Mon_AstField* Mon_AstFieldNew(const char* typeName, const char* fieldName) {
    MON_CANT_BE_NULL(typeName);
    MON_CANT_BE_NULL(fieldName);

    Mon_AstField* ret = Mon_Alloc(sizeof(Mon_AstField));
    if (ret == NULL) {
        return NULL;
    }

    ret->typeName = DuplicateString(typeName, strlen(typeName));
    if (ret->typeName == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    ret->fieldName = DuplicateString(fieldName, strlen(fieldName));
    if (ret->fieldName == NULL) {
        Mon_Free(ret->typeName);
        Mon_Free(ret);
        return NULL;
    }

    ret->semantic.type = NULL;

    return ret;
}

void Mon_AstFieldDestroy(Mon_AstField* node) {
    if (node == NULL) {
        return;
    }

    Mon_Free(node->fieldName);
    Mon_Free(node->typeName);
    Mon_Free(node);
}
