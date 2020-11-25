#ifndef MON_FUNC_DEF_H
#define MON_FUNC_DEF_H

#include <mon_defines.h>

#include <stdbool.h>

#include "ast/mon_parameter.h"
#include "ast/definitions/mon_var_def.h"
#include "ast/mon_block.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstTypeDef_ Mon_AstTypeDef;

/** Contains data that defines a function, regardless of its scope. */
typedef struct Mon_AstFuncDef_ {

    Mon_AstNodeHeader header;
    
    char*  funcName;
    size_t funcNameLength;

    /** Name of this function's return type. */
    char*  funcRetTypeName;
    size_t funcRetTypeNameLen;

    Mon_Vector parameters;

    /** The function's block of execution. NULL if the function is declared as extern. */
    Mon_AstBlock* body;

    struct {
        Mon_AstTypeDef* returnType;
        bool isEntryPoint;
    } semantic;

} Mon_AstFuncDef;

/**
 *  Constructs and returns a new function definition node.
 *
 *  @param funcName The function's name.
 *  @param funcNameLen The function's name length (excluding the null termination byte)
 *  @param funcRetType The function's return type name. If NULL, the returned type will be set to 'void'.
 *  @param funcRetTypeLen The function's return type name length. Ignored if funcRetType is NULL.
 *  @param params Vector of parameter node pointers.
 *  @param body The function's execution block node. If NULL, only contains a function declaration.
 * 
 *  @return If succesful, the new function definition node. If allocation fails,
 *  returns NULL.
 * 
 *  @remarks Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstFuncDef* MON_CALL Mon_AstFuncDefNew(const char* funcName,
                                                      size_t funcNameLen,
                                                      const char* funcRetTypeName,
                                                      size_t funcRetTypeLen,
                                                      Mon_Vector params,
                                                      Mon_AstBlock* body);

/**
 *  Destroys a function definition node, releasing its memory.
 * 
 *  @param def The definition node to be destroyed.    
 *  @param rec If true, will also recursively destroy the entire parameter and
 *  statements list.
 */
MON_PUBLIC void MON_CALL Mon_AstFuncDefDestroy(Mon_AstFuncDef* funcDef, bool rec);

C_LINKAGE_END

#endif // MON_FUNC_DEF_H