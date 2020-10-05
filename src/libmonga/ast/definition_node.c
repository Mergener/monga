#include "ast/mon_definition_node.h"

#include "mon_alloc.h"

#include "../strutils.h"

Mon_AstDef* Mon_AstVarDefNew(const char* varName,
                             size_t varNameLen,
                             const char* typeName,
                             size_t typeNameLen) {

    Mon_AstDef* ret = Mon_Alloc(sizeof(Mon_AstDef));

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

    ret->definition.variable.varName = name;
    ret->definition.variable.varNameLength = varNameLen;
    ret->definition.variable.typeName = allocTypeName;
    ret->definition.variable.typeNameLength = typeNameLen;
    ret->defKind = MON_AST_DEF_VAR;
    ret->next = NULL;

    return ret;
}

Mon_AstDef* Mon_AstFuncDefNew(const char* funcName,
                              size_t funcNameLen,
                             Mon_AstParam* firstParam) {

    Mon_AstDef* ret = Mon_Alloc(sizeof(Mon_AstDef));

    if (ret == NULL) {
        return NULL;
    }

    char* name = DuplicateString(funcName, funcNameLen);
    if (name == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    ret->defKind = MON_AST_DEF_FUNC;
    ret->definition.function.funcName = name;
    ret->definition.function.funcNameLength = funcNameLen;
    ret->definition.function.firstParam = firstParam;
    ret->next = NULL;

    return ret;
}

void Mon_AstDefDestroy(Mon_AstDef* def, bool rec) {
    switch (def->defKind) {
        case MON_AST_DEF_FUNC:
            Mon_Free(def->definition.function.funcName);
            
            if (rec) {
                Mon_AstParamDestroy(def->definition.function.firstParam, true);
            }
            break;

        case MON_AST_DEF_TYPE:
            Mon_Free(def->definition.type.typeName);
            break;
        
        case MON_AST_DEF_VAR:
            Mon_Free(def->definition.variable.typeName);
            Mon_Free(def->definition.variable.varName);
            break;

        default:
            break;
    }

    Mon_AstDef* next = def->next;
    Mon_Free(def);

    if (rec && next != NULL) {
        Mon_AstDefDestroy(next, true);
    }
}
