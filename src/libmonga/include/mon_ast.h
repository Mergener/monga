#ifndef MON_AST_H
#define MON_AST_H

#include "mon_defines.h"

#include <stdio.h>

#include "mon_error.h"
#include "mon_vector.h"
#include "mon_defgroup.h"

#include "ast/mon_block.h"
#include "ast/mon_call.h"
#include "ast/mon_cond.h"
#include "ast/mon_definition.h"
#include "ast/mon_exp.h"
#include "ast/mon_parameter.h"
#include "ast/mon_statement.h"
#include "ast/mon_var.h"
#include "ast/definitions/mon_func_def.h"
#include "ast/definitions/mon_type_def.h"
#include "ast/definitions/mon_var_def.h"
#include "ast/mon_field.h"
#include "ast/mon_typedesc.h"

C_LINKAGE_BEGIN

typedef enum {

    /** XML dump format. */
    MON_ASTDUMP_XML

} Mon_AstDumpFormat;

typedef enum {

    MON_ASTDUMP_FLAGS_NONE = 0,

    /** If set, dumps the AST in a more readable way. */
    MON_ASTDUMP_FLAGS_PRETTYPRINT = (1 << 0)

} Mon_AstDumpFlags;

typedef enum {

    MON_ASTSTATE_NONE,
    MON_ASTSTATE_SYNTAX_OK,
    MON_ASTSTATE_SEM_ERR,
    MON_ASTSTATE_SEM_OK

} Mon_AstState;

/**
 *  Abstract syntax tree for a Monga language module.
 */
typedef struct Mon_Ast_ Mon_Ast;

/**
 *  Creates an empty Monga AST, with state set to MON_ASTSTATE_NONE.
 * 
 *  @return A pointer to a newly allocated Monga AST or NULL if allocation
 *  failed.
 */
MON_PUBLIC Mon_Ast* MON_CALL Mon_AstNew(const char* moduleName);

/**
 *  Destroys an AST, releasing its resources.
 */
MON_PUBLIC void MON_CALL Mon_AstDestroy(Mon_Ast* ast);

/**
 *  Returns the state of an AST.
 * 
 *  @param ast The AST.
 * 
 *  @return The AST's current state.
 */
MON_PUBLIC Mon_AstState MON_CALL Mon_AstGetState(const Mon_Ast* ast);

/**
 *  Returns a pointer to an AST's definition group.
 * 
 *  @param ast The AST.
 * 
 *  @return The AST's definition group.
 */
MON_PUBLIC const Mon_DefGroup* MON_CALL Mon_AstGetDefinitions(const Mon_Ast* ast);

/**
 *  Adds a definition to an ast.
 *  If the AST was in a SEM_OK or SEM_ERR state, its state is returned 
 *  to SYNTAX_OK. Otherwise, its state is maintained.
 * 
 *  @param ast The AST.
 *  @param definition The definition to be added.
 * 
 *  @returns One of the following:
 *      MON_SUCCESS :: Definition added successfully.
 *      MON_ERR_NOMEM :: Allocation failed.
 */
MON_PUBLIC Mon_RetCode MON_CALL Mon_AstAddDefinition(Mon_Ast* ast, Mon_AstDef* definition);

/**
 *  Dumps an AST to a given output stream in a given format.
 *  
 *  @param ast The AST to dump.
 *  @param outputStream The stream to output the AST to.
 *  @param fmt The format to dump the AST.
 *  
 *  @return Any of the following:
 *   MON_SUCCESS :: Dumped succesfully.
 *   MON_ERR_IO :: An IO error occurred when dumping to the specified stream.
 *   MON_ERR_BAD_ARG :: Either ast was NULL, outputStream was NULL or fmt wasn't valid (part of the Mon_AstDumpFormat enumeration).
 *  
 */
MON_PUBLIC Mon_RetCode MON_CALL Mon_DumpAst(const Mon_Ast* ast, 
                                            FILE* outputStream, 
                                            Mon_AstDumpFormat fmt,
                                            Mon_AstDumpFlags flags);

/**
 *  Releases all resources used by the given AST and sets 
 *  its rootDefinition pointer to NULL.
 *
 *  @remarks Does not free the specified AST object.
 */
MON_PUBLIC void MON_CALL Mon_AstDestroy(Mon_Ast* ast);

C_LINKAGE_END

#endif // MON_AST_H