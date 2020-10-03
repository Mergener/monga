#ifndef MON_AST_H
#define MON_AST_H

#include "defines.h"

#include <stdlib.h>

//
//	Definition nodes
//

typedef enum {

	MON_AST_DEF_VAR,
	MON_AST_DEF_FUNC,
	MON_AST_DEF_TYPE

} Mon_AstDefType;

typedef struct {
	const char* varName;
	size_t      varNameLength;
} Mon_AstVarDef;

typedef struct {
	const char* funcName;
	size_t      funcNameLength;
} Mon_AstVarFunc;

typedef struct {
	const char* typeName;
	size_t      typeNameLength;
} Mon_AstVarType;

typedef struct {
	union {
		Mon_AstVarDef  variable;
		Mon_AstVarFunc function;
		Mon_AstVarType type;
	} definition;

	Mon_AstDefType defKind;

} Mon_AstDef;

#endif // MON_AST_H