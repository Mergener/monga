#ifndef MON_FUNC_DEF_H
#define MON_FUNC_DEF_H

#include <mon_defines.h>

#include "ast/mon_parameter.h"

C_LINKAGE_BEGIN

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

/**
 *	Constructs and returns a new function definition node.
 *
 * 	@param funcName The function's name.
 * 	@param funcNameLen The function's name length (excluding the null termination byte)
 * 	@param firstParam Pointer to the first parameter node. May be NULL if function has no parameters.
 * 
 * 	@return If succesful, the new function definition node. If allocation fails,
 * 	returns NULL.
 */
MON_PUBLIC Mon_AstFuncDef* MON_CALL Mon_AstFuncDefNew(const char* funcName,
                                                      size_t funcNameLen,
                                                      Mon_AstParam* firstParam);

/**
 *  Destroys a function definition node, releasing its memory.
 * 
 *  @param def The definition node to be destroyed.	
 * 	@param rec If true, will also recursively destroy the entire parameter and
 * 	statements list.
 */
MON_PUBLIC void MON_CALL Mon_AstFuncDefDestroy(Mon_AstFuncDef* funcDef, bool rec);

C_LINKAGE_END

#endif // MON_FUNC_DEF_H