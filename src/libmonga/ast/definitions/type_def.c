#include "ast/definitions/mon_type_def.h"

#include <mon_debug.h>

#include "mon_alloc.h"
#include "../../strutils.h"

Mon_AstTypeDef* Mon_AstTypeDefNew(const char* name,
                                  size_t nameLen) {

    MON_CANT_BE_NULL(name);

	Mon_AstTypeDef* ret = Mon_Alloc(sizeof(Mon_AstTypeDef));

    if (ret == NULL) {
        return NULL;
    }

    ret->typeName = DuplicateString(name, nameLen);
    if (ret->typeName == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    ret->typeNameLength = nameLen;

    return ret;
}

void Mon_AstTypeDefDestroy(Mon_AstTypeDef* def) {
    if (def == NULL) {
        return;
    }

	Mon_Free(def->typeName);
	Mon_Free(def);
}