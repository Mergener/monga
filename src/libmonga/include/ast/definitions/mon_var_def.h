#ifndef MON_VAR_DEF_H
#define MON_VAR_DEF_H

#include <mon_defines.h>

#include <stdlib.h>

C_LINKAGE_BEGIN

/** Contains data that defines a variable, regardless of its scope. */
typedef struct Mon_AstVarDef_ {

    Mon_AstNodeHeader header;
    
    char*  varName;
    size_t varNameLength;

    char* typeName;
    size_t typeNameLength;

    struct {
        Mon_AstTypeDef* type;
    } semantic;

} Mon_AstVarDef;

/**
 *  Constructs and returns a variable definition node.
 *
 *  @param varName The variable's name.
 *  @param varNameLen The variable's name length (excluding the null termination byte).
 *  @param typeName The variable's type name.
 *  @param typeNameLen The variable's type name length (excluding the null termination byte).
 * 
 *  @returns The created variable definition node or NULL if allocation failed.
 * 
 *  @remarks The 'next' node pointer is initialized to NULL.
 *           Semantic data is initialized to NULL.
 */
MON_PUBLIC Mon_AstVarDef* MON_CALL Mon_AstVarDefNew(const char* varName,
                                                    size_t varNameLen,
                                                    const char* typeName,
                                                    size_t typeNameLen);

/**
 *  Destroys a variable definition node, releasing its memory.
 *  Does nothing if the specified node is NULL.
 *
 *  @param varDefNode The variable definition node to be destroyed.
 */
MON_PUBLIC void MON_CALL Mon_AstVarDefDestroy(Mon_AstVarDef* varDefNode);

C_LINKAGE_END

#endif // MON_VAR_DEF_H