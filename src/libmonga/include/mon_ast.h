#ifndef MON_AST_H
#define MON_AST_H

#include "mon_defines.h"

#include <stdio.h>

#include "mon_error.h"
#include "mon_vector.h"

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
    Mon_Vector defsVector;

    Mon_AstState astState;

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

typedef struct {

    bool (*blockVisitor)(void* ctx, Mon_AstBlock* node, Mon_AstVisitDirection dir);
    bool (*callVisitor)(void* ctx, Mon_AstCall* node, Mon_AstVisitDirection dir);
    bool (*condVisitor)(void* ctx, Mon_AstCond* node, Mon_AstVisitDirection dir);
    bool (*defVisitor)(void* ctx, Mon_AstDef* node, Mon_AstVisitDirection dir);
    bool (*expVisitor)(void* ctx, Mon_AstExp* node, Mon_AstVisitDirection dir);
    bool (*paramVisitor)(void* ctx, Mon_AstParam* node, Mon_AstVisitDirection dir);
    bool (*stmtVisitor)(void* ctx, Mon_AstStatement* node, Mon_AstVisitDirection dir);
    bool (*varVisitor)(void* ctx, Mon_AstVar* node, Mon_AstVisitDirection dir);
    bool (*funcDefVisitor)(void* ctx, Mon_AstFuncDef* node, Mon_AstVisitDirection dir);
    bool (*typeDefVisitor)(void* ctx, Mon_AstTypeDef* node, Mon_AstVisitDirection dir);
    bool (*varDefVisitor)(void* ctx, Mon_AstVarDef* node, Mon_AstVisitDirection dir);
    bool (*fieldVisitor)(void* ctx, Mon_AstField* node, Mon_AstVisitDirection dir);
    bool (*typeDescVisitor)(void* ctx, Mon_AstTypeDesc* node, Mon_AstVisitDirection dir);

} Mon_AstVisitor;

/**
 *  Initializes an AST visitor object, setting all fields to NULL.
 * 
 *  @param vis The visitor to be initialized.
 * 
 *  @remarks It is important to call this method before using the visitor,
 *  even when all fields should be replaced with another value. This guarantees
 *  that your code won't behave in an undefined manner if new visitor function
 *  fields are to be added in the future, since this function will always make
 *  sure that all of them are initialized to NULL.
 */
MON_PUBLIC void MON_CALL Mon_InitializeAstVisitor(Mon_AstVisitor* vis);

typedef enum {

    MON_ASTVIS_BOTTOMUP = 1,
    MON_ASTVIS_TOPDOWN = 2,
    MON_ASTVIS_BOTH = MON_ASTVIS_BOTTOMUP | MON_ASTVIS_TOPDOWN

} Mon_AstVisitDirection;

/**
 *  Visits the specified AST, iterating through each node.
 *  
 *  @param ast The ast to visit.
 *  @param visitor A container of functions to call for each visited node.
 *  Fields pointing to NULL are not called. Three arguments are passed to each
 *  function:
 *      ctx - A context pointer. The same one that is passed to Mon_VisitAst.
 *      node - The node being visited.
 *      dir - The current direction of the iteration (either bottom up or top down).
 *  Each function should either return true or false. If any of them returns false,
 *  the tree iteration is interrupted.
 *  @param context A generic pointer to a user-defined context object. This object
 *  will be passed to the 'ctx' parameter for each called visitor function.
 *  @param direction The direction of the visit. If MON_ASTVIS_TOPDOWN, the visit
 *  will be descending, starting from the topmost node and calling its callback and
 *  terminating on the leaves. If MON_ASTVIS_BOTTOMUP, visit will start from the leaves
 *  and finish on the AST root. If MON_ASTVIS_BOTH, each function will be called two times.
 *  The first time will be the moment where the visit is descending from the root node, and
 *  a second time for the moment the visit is ascending once again to the top.
 * 
 *  @return True if no called function returned false, false otherwise.
 */
MON_PUBLIC bool MON_CALL Mon_VisitAst(Mon_Ast* ast, 
                                      const Mon_AstVisitor* visitor, 
                                      void* context,
                                      Mon_AstVisitDirection direction);

C_LINKAGE_END

#endif // MON_AST_H