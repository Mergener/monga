#ifndef SYMBOL_H
#define SYMBOL_H

#include <mon_defines.h>

#include "mon_vector.h"
#include "mon_ast.h"

typedef enum {
    
    SYM_PARAM,
    SYM_FUNC,
    SYM_VAR,
    SYM_TYPE

} SymbolKind;

/**
 * 	Refers to a symbol declaration. A symbol is a language construct that
 * 	is associated with a user provided name. Special cases of symbols are
 * 	builtin types (int, float...). Those are symbols as well, but their
 * 	names exist regardless of declaration, since they are specified by the
 * 	language itself.
 */
typedef struct {

    /** This symbol kind. Specifies the active field in the 'definition' union. */
    SymbolKind kind;

    /** Pointer to the AST node that defines the properties of this symbol. */
    union {
        /** A function parameter variable symbol. Available if kind == SYM_PARAM. */
        Mon_AstParam*   param;

        /** A function declaration symbol. Available if kind == SYM_FUNC. */
        Mon_AstFuncDef* func;

        /** A variable symbol. Available if kind == SYM_VAR. */
        Mon_AstVarDef*  var;

        /** A type symbol. Available if kind == SYM_TYPE. */
        Mon_AstTypeDef* type;
    } definition;

} Symbol;

MON_PRIVATE Symbol* NewParamSymbol(Mon_AstParam* param);
MON_PRIVATE Symbol* NewFuncSymbol(Mon_AstFuncDef* func);
MON_PRIVATE Symbol* NewTypeSymbol(Mon_AstTypeDef* type);
MON_PRIVATE Symbol* NewVarSymbol(Mon_AstVarDef* var);

/**
 * 	Returns the name of a symbol.
 */
MON_PRIVATE const char* GetSymbolName(const Symbol* s);

/**
 * 	Returns a non-capitalized string representation of a symbol kind.
 */
MON_PRIVATE const char* GetSymbolKindName(SymbolKind kind);

/**
 * 	Destroys the specified symbol object.
 * 	Does _NOT_ destroy AST nodes pointed by it.
 */
MON_PRIVATE void DestroySymbol(Symbol* s);

#endif // SYMBOL_H