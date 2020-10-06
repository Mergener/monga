#ifndef MON_FUNC_DEF_H
#define MON_FUNC_DEF_H

#include <mon_defines.h>

#include "ast/mon_parameter.h"
#include "ast/definitions/mon_var_def.h"
#include "ast/mon_block.h"

C_LINKAGE_BEGIN

/** Contains data that defines a function, regardless of its scope. */
typedef struct {
    char*  funcName;
    size_t funcNameLength;

    /** Name of this function's return type. */
    char*  funcRetTypeName;
    size_t funcRetTypeNameLen;

    /** 
     *  Pointer to the first parameter node in a null-terminated chain of nodes. 
     *  NULL if the function has no parameters.
     */
    Mon_AstParam* firstParam;

    Mon_AstBlock* body;

} Mon_AstFuncDef;

/**
 *	Constructs and returns a new function definition node.
 *
 * 	@param funcName The function's name.
 * 	@param funcNameLen The function's name length (excluding the null termination byte)
 *  @param funcRetType The function's return type name. If NULL, the returned type will be set to 'void'.
 *  @param funcRetTypeLen The function's return type name length. Ignored if funcRetType is NULL.
 * 	@param firstParam Pointer to the first parameter node. May be NULL if function has no parameters.
 * 
 * 	@return If succesful, the new function definition node. If allocation fails,
 * 	returns NULL.
 */
MON_PUBLIC Mon_AstFuncDef* MON_CALL Mon_AstFuncDefNew(const char* funcName,
                                                      size_t funcNameLen,
                                                      const char* funcRetTypeName,
                                                      size_t funcRetTypeLen,
                                                      Mon_AstParam* firstParam,
                                                      Mon_AstBlock* body);

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