#include "ast/mon_parameter.h"

#include "mon_alloc.h"
#include "../strutils.h"

Mon_AstParam* Mon_AstParamNew(const char* name,
                              size_t nameLen,
                              const char* typeName,
                              size_t typeNameLen) {
    
    Mon_AstParam* ret = Mon_Alloc(sizeof(Mon_AstParam));
    if (ret == NULL) {
        return NULL;
    }

    char* parName = DuplicateString(name, nameLen);
    if (parName == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    char* parTypeName = DuplicateString(typeName, typeNameLen);
    if (parTypeName == NULL) {
        Mon_Free(parTypeName);
        Mon_Free(ret);
        return NULL;
    }

    ret->name = parName;
    ret->nameLen = nameLen;
    ret->typeName = parTypeName;
    ret->typeNameLen = typeNameLen;
    ret->next = NULL;

    return ret;
}

void Mon_AstParamDestroy(Mon_AstParam* param, bool rec) {
    if (rec) {
        Mon_AstParam* curr = param->next;

        while (curr != NULL) {
            Mon_AstParamDestroy(curr, false);
            curr = curr->next;
        }
    }

    Mon_Free(param->name);
    Mon_Free(param->typeName);
    Mon_Free(param);
}