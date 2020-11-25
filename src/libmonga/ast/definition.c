#include "ast/mon_definition.h"

#include "mon_debug.h"
#include "mon_alloc.h"

static Mon_AstDef* MakeDefNode(void* defPtr, Mon_AstDefKind defKind) { 
    Mon_AstDef* ret = Mon_Alloc(sizeof(Mon_AstDef));

    if (ret == NULL) {
        return NULL;
    }

    ret->defKind = defKind;
    ret->definition.ptr = defPtr;

    return ret;
}

Mon_AstDef* Mon_AstDefNewVar(Mon_AstVarDef* varDef) {
    Mon_AstDef* ret = MakeDefNode(varDef, MON_AST_DEF_VAR);
    ret->name = varDef->varName;
    return ret;
}

Mon_AstDef* Mon_AstDefNewFunc(Mon_AstFuncDef* funcDef) {
    Mon_AstDef* ret = MakeDefNode(funcDef, MON_AST_DEF_FUNC);
    ret->name = funcDef->funcName;
    return ret;
}

Mon_AstDef* Mon_AstDefNewType(Mon_AstTypeDef* typeDef) {
    Mon_AstDef* ret =  MakeDefNode(typeDef, MON_AST_DEF_TYPE);
    ret->name = typeDef->typeName;
    return ret;
}

void Mon_AstDefDestroy(Mon_AstDef* def, bool rec) {
    if (def == NULL) {
        return;
    }

    if (rec) {
        switch (def->defKind) {
            case MON_AST_DEF_FUNC:
                Mon_AstFuncDefDestroy(def->definition.function, true);
                break;

            case MON_AST_DEF_VAR:
                Mon_AstVarDefDestroy(def->definition.variable);
                break;

            case MON_AST_DEF_TYPE:
                Mon_AstTypeDefDestroy(def->definition.type, true);
                break;

            default:
                MON_ASSERT(false, "Unimplemented defKind. (got %d)", (int)def->defKind);
                break;
        }
    }

    Mon_Free(def);
}