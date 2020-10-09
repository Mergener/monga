#include "ast/definitions/mon_var_def.h"

#include <assert.h>
#include "mon_alloc.h"
#include "../../strutils.h"

Mon_AstVarDef* Mon_AstVarDefNew(const char* varName,
                             size_t varNameLen,
                             const char* typeName,
                             size_t typeNameLen) {

    assert(varName != NULL);
    assert(typeName != NULL);

    Mon_AstVarDef* ret = Mon_Alloc(sizeof(Mon_AstVarDef));

    if (ret == NULL) {
        return NULL;
    }

    char* name = DuplicateString(varName, varNameLen);
    if (name == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    char* allocTypeName = DuplicateString(typeName, typeNameLen);
    if (allocTypeName == NULL) {
        Mon_Free(name);
        Mon_Free(ret);
        return NULL;
    }

    ret->varName = name;
    ret->varNameLength = varNameLen;
    ret->typeName = allocTypeName;
    ret->typeNameLength = typeNameLen;

    return ret;
}

void Mon_AstVarDefDestroy(Mon_AstVarDef* varDefNode) {
    if (varDefNode == NULL) {
        return;
    }

	Mon_Free(varDefNode->typeName);
	Mon_Free(varDefNode->varName);
	Mon_Free(varDefNode);
}