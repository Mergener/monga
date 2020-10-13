#include "ast/definitions/mon_type_def.h"

#include <mon_debug.h>

#include "mon_alloc.h"
#include "../../strutils.h"
#include "ast/mon_typedesc.h"

Mon_AstTypeDef* Mon_AstTypeDefNew(const char* name,
                                  size_t nameLen,
                                  Mon_AstTypeDesc* typeDesc) {

    MON_CANT_BE_NULL(name);
    MON_CANT_BE_NULL(typeDesc);

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
    ret->typeDesc = typeDesc;

    return ret;
}

void Mon_AstTypeDefDestroy(Mon_AstTypeDef* def, bool rec) {
    if (def == NULL) {
        return;
    }

    Mon_AstTypeDef defVal = *def;

	Mon_Free(def->typeName);
	Mon_Free(def);

    if (rec) {
        Mon_AstTypeDescDestroy(defVal.typeDesc, true);
    }
}