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

/** Contains data that defines a variable, regardless of its scope. */
typedef struct {
	const char* varName;
	size_t      varNameLength;
} Mon_AstVarDef;

/** Contains data that defines a function, regardless of its scope. */
typedef struct {
	const char* funcName;
	size_t      funcNameLength;
} Mon_AstFuncDef;

/** Contains data that defines a type, regardless of its scope. */
typedef struct {
	const char* typeName;
	size_t      typeNameLength;
} Mon_AstTypeDef;

typedef struct {

	union {

		/** Available if defKind == MON_AST_DEF_VAR */
		Mon_AstVarDef  variable;

		/** Available if defKind == MON_AST_DEF_FUNC */
		Mon_AstFuncDef function;

		/** Available if defKind == MON_AST_DEF_TYPE */
		Mon_AstTypeDef type;

	} definition;

	/** This node definition kind. Indicates what is this definition node is actually defining. */
	Mon_AstDefKind defKind;

	/** Next definition. May be NULL. */
	Mon_AstDef* next;

} Mon_AstDef;

/**
 *	Constructs and returns a definition node for a variable definition.
 *
 * 	@param funcName The variable's name.
 * 	@param funcNameLen The variable's name length (excluding the null termination byte)
 * 
 * 	@return If succesful, returns a definition node, with defKind set to MON_AST_DEF_VAR and definition 
 * 	field filled with 'variable' data. If allocation fails, returns NULL.
 * 
 * 	@remarks The 'next' node pointer is initialized to NULL.
 * 
 */
MON_API Mon_AstDef* MON_CALL Mon_AstVarDefNew(const char* varName,
                                               size_t varNameLen);

/**
 *	Constructs and returns a definition node for a function definition.
 *
 * 	@param funcName The function's name.
 * 	@param funcNameLen The function's name length (excluding the null termination byte)
 * 
 * 	@return If succesful, returns a definition node, with defKind set to MON_AST_DEF_FUNC and definition 
 * 	field filled with 'function' data. If allocation fails, returns NULL.
 * 
 * 	@remarks The 'next' node pointer is initialized to NULL.
 * 
 */
MON_API Mon_AstDef* MON_CALL Mon_AstFuncDefNew(const char* funcName,
                                               size_t funcNameLen);

C_LINKAGE_END

#endif // MON_AST_DEFINITION_NODE_H