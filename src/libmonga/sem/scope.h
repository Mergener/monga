#ifndef SCOPE_H
#define SCOPE_H

#include <mon_defines.h>

#include "mon_error.h"
#include "mon_vector.h"

typedef struct Symbol_ Symbol;

/**
 * 	A scope contains symbols. Semantically, a scope constrains the group
 * 	of symbols acessible from a certain portion of the code. Scopes can be
 * 	nested, and thus, enclosed by a greater scope.
 */
typedef struct Scope_ {

    struct Scope_* parentScope;
    Mon_Vector     symbols;

} Scope;

/**
 *  Creates a new scope object. Returns null if
 *  an allocation fails.
 */
MON_PRIVATE Scope* NewScope();

/**
 *  Searches for a symbol with the specified name in the scope. 
 *  Returns a pointer to the found symbol or NULL if none is found.
 *  If 'rec' is set, searches for parent scopes and the builtin scope
 *  as well.
 *  If the specified scope is NULL, searches for the symbol in the
 *  builtin scope.
 */
MON_PRIVATE Symbol* FindSymbolInScope(const Scope* s, const char* name, bool rec);

/**
 *  Tries to add a symbol to the scope.
 *  If successful, returns MON_SUCCESS.
 *  If symbol already exists in that scope, returns MON_ERR_DUPLICATE.
 *  If symbol already exists in the builtin scope, returns MON_ERR_BUILTIN.
 *  If an allocation fails, returns MON_ERR_NOMEM.
 */
MON_PRIVATE Mon_RetCode TryAddSymbolToScope(Scope* s, Symbol* sym);

/**
 *  Forces the integration of a symbol to a scope, without
 *  checking for name clashes.
 *  Returns false if an allocation fails, true otherwise.
 *  This might cause undesired behaviour if a symbol with
 *  the same name exists in the specified scope.
 *  In debug mode (MON_DEBUG), this function will still search
 *  for the symbol in the specified scope and throw an
 *  assertion error if it is found there.
 */
MON_PRIVATE bool ForceAddSymbolToScope(Scope* s, Symbol* sym);

/**
 * 	Destroys a scope, releasing its memory.
 */
MON_PRIVATE void DestroyScope(Scope* s);

#endif // SCOPE_H