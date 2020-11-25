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
typedef struct {

    /** Vector containing all program definitions. Stored elements are of type Mon_AstDef*. */
    Mon_DefGroup definitions;

    Mon_AstState astState;

    char* moduleName;

    struct {
        /** Contains the list types used by this module. */
        Mon_Vector usedTypes;
    } semantic;

} Mon_Ast;

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
MON_PUBLIC void MON_CALL Mon_AstFinalize(Mon_Ast* ast);

C_LINKAGE_END

#endif // MON_AST_H