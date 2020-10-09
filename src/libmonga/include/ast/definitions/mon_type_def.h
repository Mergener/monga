#ifndef MON_TYPE_DEF_H
#define MON_TYPE_DEF_H

#include <mon_defines.h>

#include <stdlib.h>

C_LINKAGE_BEGIN

/** Contains data that defines a type, regardless of its scope. */
typedef struct Mon_AstTypeDef_ {
    char*  typeName;
    size_t typeNameLength;
} Mon_AstTypeDef;

/**
 *	Constructs and returns a new type definition node.
 *
 * 	@param name The function's name.
 * 	@param nameLen The function's name length (excluding the null termination byte)
 * 
 * 	@return If succesful, the new type definition node. If allocation fails,
 * 	returns NULL.
 */
MON_PUBLIC Mon_AstTypeDef* MON_CALL Mon_AstTypeDefNew(const char* name,
                                                      size_t nameLen);

/**
 *  Destroys a type definition node, releasing its memory.
 * 
 *  @param def The definition node to be destroyed.
 */
MON_PUBLIC void MON_CALL Mon_AstTypeDefDestroy(Mon_AstTypeDef* def);

C_LINKAGE_END

#endif // MON_TYPE_DEF_H