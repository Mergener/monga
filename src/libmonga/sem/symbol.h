#ifndef SYMBOL_H
#define SYMBOL_H

#include <mon_defines.h>

#include "mon_vector.h"
#include "mon_ast.h"

/**
 * 	Refers to a symbol declaration. A symbol is a language construct that
 * 	is associated with a user provided name. Special cases of symbols are
 * 	builtin types (int, float...). Those are symbols as well, but their
 * 	names exist regardless of declaration, since they are specified by the
 * 	language itself.
 */
typedef struct {

	/** This symbol kind. Specifies the active field in the 'definition' union. */
	enum {		
		SYM_PARAM,
		SYM_FUNC,
		SYM_VAR,
		SYM_TYPE
	} kind;

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

/**
 * 	Returns the name of a symbol.
 */
MON_PRIVATE const char* GetSymbolName(const Symbol* s);

#endif // SYMBOL_H