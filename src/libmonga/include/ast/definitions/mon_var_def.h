#ifndef MON_VAR_DEF_H
#define MON_VAR_DEF_H

#include <mon_defines.h>

#include <stdlib.h>

C_LINKAGE_BEGIN

/** Contains data that defines a variable, regardless of its scope. */
typedef struct {
    
    char*  varName;
    size_t varNameLength;

    char* typeName;
    size_t typeNameLength;

} Mon_AstVarDef;

/**
 *	Constructs and returns a variable definition node.
 *
 * 	@param funcName The variable's name.
 * 	@param funcNameLen The variable's name length (excluding the null termination byte)
 * 
 * 	@returns The created variable definition node or NULL if allocation failed.
 * 
 * 	@remarks The 'next' node pointer is initialized to NULL.
 */
MON_PUBLIC Mon_AstVarDef* MON_CALL Mon_AstVarDefNew(const char* varName,
                                                    size_t varNameLen,
                                                    const char* typeName,
                                                    size_t typeNameLen);

/**
 *	Destroys a variable definition node, releasing its memory.
 *
 * 	@param varDefNode The variable definition node to be destroyed.
 */
MON_PUBLIC void MON_CALL Mon_AstVarDefDestroy(Mon_AstVarDef* varDefNode);

C_LINKAGE_END

#endif // MON_VAR_DEF_H