#ifndef MON_AST_DEFINITION_NODE_H
#define MON_AST_DEFINITION_NODE_H

#include <mon_defines.h>

#include <stdlib.h>

#include "definitions/mon_func_def.h"
#include "definitions/mon_type_def.h"
#include "definitions/mon_var_def.h"

#include "ast/mon_astbase.h"

C_LINKAGE_BEGIN

typedef enum {

    /** Variable definition node. */
    MON_AST_DEF_VAR,

    /** Function definition node. */
    MON_AST_DEF_FUNC,

    /** Type definition node. */
    MON_AST_DEF_TYPE

} Mon_AstDefKind;

typedef struct Mon_AstDef_ {

    Mon_AstNodeHeader header;

    /** This node definition kind. Indicates what is this definition node is actually defining. */
    Mon_AstDefKind defKind;

    union {

        /** 
         *  A variable declaration node.
         *  Available if defKind == MON_AST_DEF_VAR 
         */
        Mon_AstVarDef* variable;

        /** 
         *  A function declaration and implementation node.
         *  Available if defKind == MON_AST_DEF_FUNC
         */
        Mon_AstFuncDef* function;

        /** 
         *  A type declaration node.
         *  Available if defKind == MON_AST_DEF_TYPE
         */
        Mon_AstTypeDef* type;

        /** Generic pointer handle to the definition node. */
        void* ptr;

    } definition;

    const char* name;

} Mon_AstDef;

/**
 *  Creates a new definition node from a variable defition node.
 *  The returned definition node will have its defKind field set to MON_AST_DEF_VAR.
 * 
 *  @param varDef The variable definition node.
 * 
 *  @return The created definition node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstDef* MON_CALL Mon_AstDefNewVar(Mon_AstVarDef* varDef);

/**
 *  Creates a new definition node from a function defition node.
 *  The returned definition node will have its defKind field set to MON_AST_DEF_FUNC.
 * 
 *  @param funcDef The function definition node.
 * 
 *  @return The created definition node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstDef* MON_CALL Mon_AstDefNewFunc(Mon_AstFuncDef* funcDef);

/**
 *  Creates a new definition node from a type defition node.
 *  The returned definition node will have its defKind field set to MON_AST_DEF_TYPE.
 * 
 *  @param typeDef The type definition node.
 * 
 *  @return The created definition node or NULL if allocation fails.
 */
MON_PUBLIC Mon_AstDef* MON_CALL Mon_AstDefNewType(Mon_AstTypeDef* typeDef);

/**
 *  Destroys a definition node, releasing its memory.
 * 
 *  @param def The definition node to be destroyed.
 *  @param rec If true, destroys all subtrees being referenced by this definition node.
 */
MON_PUBLIC void MON_CALL Mon_AstDefDestroy(Mon_AstDef* def, bool rec);

C_LINKAGE_END

#endif // MON_AST_DEFINITION_NODE_H