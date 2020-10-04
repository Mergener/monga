#ifndef MON_AST_DEFINITION_NODE_H
#define MON_AST_DEFINITION_NODE_H

#include "../defines.h"

#include <stdlib.h>

C_LINKAGE_BEGIN

typedef enum {

	/** Variable definition node. */
	MON_AST_DEF_VAR,

	/** Function definition node. */
	MON_AST_DEF_FUNC,

	/** Type definition node. */
	MON_AST_DEF_TYPE

} Mon_AstDefKind;

typedef struct {
	const char* varName;
	size_t      varNameLength;
} Mon_AstVarDef;

typedef struct {
	const char* funcName;
	size_t      funcNameLength;
} Mon_AstFuncDef;

typedef struct {
	const char* typeName;
	size_t      typeNameLength;
} Mon_AstTypeDef;

typedef struct {
	union {
		Mon_AstVarDef  variable;
		Mon_AstFuncDef function;
		Mon_AstTypeDef type;
	} definition;

	Mon_AstDefKind defKind;

} Mon_AstDef;



C_LINKAGE_END

#endif // MON_AST_DEFINITION_NODE_H