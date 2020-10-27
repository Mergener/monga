#include "symbol.h"

#include "mon_alloc.h"
#include "mon_debug.h"

Symbol* NewParamSymbol(Mon_AstParam* param) {
    MON_CANT_BE_NULL(param);

    Symbol* ret = Mon_Alloc(sizeof(Symbol));
    if (ret == NULL) {
        return NULL;
    }

    ret->kind = SYM_PARAM;
    ret->definition.param = param;

    return ret;
}

Symbol* NewFuncSymbol(Mon_AstFuncDef* func) {
    MON_CANT_BE_NULL(func);

    Symbol* ret = Mon_Alloc(sizeof(Symbol));
    if (ret == NULL) {
        return NULL;
    }

    ret->kind = SYM_FUNC;
    ret->definition.func = func;

    return ret;
}

Symbol* NewTypeSymbol(Mon_AstTypeDef* type) {
    MON_CANT_BE_NULL(type);

    Symbol* ret = Mon_Alloc(sizeof(Symbol));
    if (ret == NULL) {
        return NULL;
    }

    ret->kind = SYM_TYPE;
    ret->definition.type = type;

    return ret;
}

Symbol* NewVarSymbol(Mon_AstVarDef* var) {
    MON_CANT_BE_NULL(var);

    Symbol* ret = Mon_Alloc(sizeof(Symbol));
    if (ret == NULL) {
        return NULL;
    }

    ret->kind = SYM_VAR;
    ret->definition.var = var;

    return ret;
}

void DestroySymbol(Symbol* s) {
    if (s == NULL) {
        return;
    }

    Mon_Free(s);
}

const char* GetSymbolName(const Symbol* s) {
    MON_CANT_BE_NULL(s);

    switch (s->kind) {
        case SYM_PARAM:
            return s->definition.param->name;

        case SYM_FUNC:
            return s->definition.func->funcName;

        case SYM_VAR:
            return s->definition.var->varName;

        case SYM_TYPE:
            return s->definition.type->typeName;

        default:
            MON_ASSERT(false, "Unimplemented symbol kind. (got %d)", (int)s->kind);
            return "";
    }
}

const char* GetSymbolKindName(SymbolKind kind) {
    switch (kind) {
        case SYM_PARAM:
            return "parameter";

        case SYM_FUNC:
            return "function";

        case SYM_VAR:
            return "variable";

        case SYM_TYPE:
            return "type";

        default:
            MON_ASSERT(false, "Unimplemented symbol kind. (got %d)", kind);
            return "";
    }
}