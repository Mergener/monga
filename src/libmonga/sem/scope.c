#include "scope.h"

#include <string.h>

#include "symbol.h"

#include "builtins.h"
#include "mon_debug.h"
#include "mon_alloc.h"

Scope* NewScope() {
    Scope* s = Mon_Alloc(sizeof(Scope));
    if (s == NULL) {
        return NULL;
    }

    if (Mon_VectorInit(&s->symbols) != MON_SUCCESS) {
        Mon_Free(s);
        return NULL;
    }

    s->parentScope = NULL;
    return s;
}

Symbol* FindSymbolInScope(const Scope* s, const char* name, bool rec) {
    MON_CANT_BE_NULL(name);

    if (s == NULL) {
        return FindSymbolInScope(BUILTIN_SCOPE, name, false);
    }

    MON_VECTOR_FOREACH(&s->symbols, Symbol*, sym,
        if (strcmp(sym->symName, name) == 0) {
            return sym;
        }
    );

    if (rec) {
        return FindSymbolInScope(s->parentScope, name, true);
    }
    return NULL;
}

static bool AddSymbolInternal(Scope* s, Symbol* sym) {
    MON_CANT_BE_NULL(s);
    MON_CANT_BE_NULL(sym);

    return Mon_VectorPush(&s->symbols, sym) == MON_SUCCESS;
}

bool ForceAddSymbolToScope(Scope* s, Symbol* sym) {
    MON_CANT_BE_NULL(s);
    MON_CANT_BE_NULL(sym);

#ifdef MON_DEBUG
    Mon_RetCode ret = TryAddSymbolToScope(s, sym);

    MON_ASSERT(ret != MON_ERR_DUPLICATE,
        "Cannot call ForceAddSymbolToScope on scope that already contains "
        "a symbol with the specified name. (tried to add '%s')",
        sym->symName);
    
    return ret == MON_SUCCESS;    
#else
    return AddSymbolInternal(s, sym);
#endif
}

Mon_RetCode TryAddSymbolToScope(Scope* s, Symbol* sym) {
    MON_CANT_BE_NULL(s);
    MON_CANT_BE_NULL(sym);

    Symbol* existing = FindSymbolInScope(s, sym->symName, false);
    if (existing != NULL) {
        return MON_ERR_DUPLICATE;
    }

    existing = FindSymbolInScope(BUILTIN_SCOPE, sym->symName, false);
    if (existing != NULL) {
        return MON_ERR_BUILTIN;
    }

    return AddSymbolInternal(s, sym) ? MON_SUCCESS : MON_ERR_NOMEM;
}

void DestroyScope(Scope* s) {
    MON_CANT_BE_NULL(s);
    MON_VECTOR_FOREACH(&s->symbols, Symbol*, sym, 
        MON_CANT_BE_NULL(sym);
        DestroySymbol(sym);
    );
    Mon_VectorFinalize(&s->symbols);
    Mon_Free(s);
}