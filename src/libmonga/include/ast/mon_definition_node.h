#ifndef MON_AST_DEFINITION_NODE_H
#define MON_AST_DEFINITION_NODE_H

#include "../mon_defines.h"

#include <stdlib.h>

#include "mon_parameter.h"

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
    
    char*  varName;
    size_t varNameLength;

    char* typeName;
    size_t typeNameLength;

} Mon_AstVarDef;

/** Contains data that defines a function, regardless of its scope. */
typedef struct {
    char*  funcName;
    size_t funcNameLength;

    /** 
     * Pointer to the first parameter node in a null-terminated chain of nodes. 
     * NULL if the function has no parameters.
     */
    Mon_AstParam* firstParam;
} Mon_AstFuncDef;

/** Contains data that defines a type, regardless of its scope. */
typedef struct {
    char*  typeName;
    size_t typeNameLength;
} Mon_AstTypeDef;

typedef struct Mon_AstDef_ {

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
    struct Mon_AstDef_* next;

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
MON_PUBLIC Mon_AstDef* MON_CALL Mon_AstVarDefNew(const char* varName,
                                                 size_t varNameLen,
                                                 const char* typeName,
                                                 size_t typeNameLen);

/**
 *	Constructs and returns a definition node for a function definition.
 *
 * 	@param funcName The function's name.
 * 	@param funcNameLen The function's name length (excluding the null termination byte)
 * 	@param firstParam Pointer to the first parameter node. May be NULL if function has no parameters.
 * 
 * 	@return If succesful, returns a definition node, with defKind set to MON_AST_DEF_FUNC and definition 
 * 	field filled with 'function' data. If allocation fails, returns NULL.
 * 
 * 	@remarks The 'next' node pointer is initialized to NULL.
 * 
 */
MON_PUBLIC Mon_AstDef* MON_CALL Mon_AstFuncDefNew(const char* funcName,
                                                  size_t funcNameLen,
                                                  Mon_AstParam* firstParam);

/**
 *  Destroys a definition node, releasing its memory.
 * 
 *  @param def The definition node to be destroyed.
 *  @param rec If true, all the definition nodes in the definition chain will be destroyed,
 *  as well as any AST nodes in the def's subtree. If false, only def's own resources are deallocated.
 */
MON_PUBLIC void MON_CALL Mon_AstDefDestroy(Mon_AstDef* def, bool rec);

C_LINKAGE_END

#endif // MON_AST_DEFINITION_NODE_H