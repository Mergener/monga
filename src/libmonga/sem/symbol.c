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
    ret->symName = param->name;

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
    ret->symName = func->funcName;

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
    ret->symName = type->typeName;

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
    ret->symName = var->varName;

    return ret;
}

Symbol* NewFieldSymbol(Mon_AstField* field) {
    MON_CANT_BE_NULL(field);

    Symbol* ret = Mon_Alloc(sizeof(Symbol));
    if (ret == NULL) {
        return NULL;
    }

    ret->kind = SYM_FIELD;
    ret->definition.field = field;
    ret->symName = field->fieldName;

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

    return s->symName;
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

        case SYM_FIELD:
            return "field";

        default:
            MON_ASSERT(false, "Unimplemented symbol kind. (got %d)", kind);
            return "";
    }
}