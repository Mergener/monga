#include "ast/mon_definition.h"

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
    return MakeDefNode(varDef, MON_AST_DEF_VAR);   
}

Mon_AstDef* Mon_AstDefNewFunc(Mon_AstFuncDef* funcDef) {
    return MakeDefNode(funcDef, MON_AST_DEF_FUNC);
}

Mon_AstDef* Mon_AstDefNewType(Mon_AstTypeDef* typeDef) {
    return MakeDefNode(typeDef, MON_AST_DEF_TYPE);
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
                Mon_AstTypeDefDestroy(def->definition.type);
                break;
        }
    }

    Mon_Free(def);
}