#ifndef SEM_H
#define SEM_H

#include <mon_defines.h>

#include <stdbool.h>
#include <stdio.h>

#include "mon_error.h"
#include "mon_ast.h"

C_LINKAGE_BEGIN

/**
 * 	Performs semantic analysis on the specified abstract syntax tree,
 * 	filling semantic fields in each of its nodes.
 * 
 * 	@param ast The AST to analyse.
 * 	@param semErrOutStream The output stream to direct semantic error messages to.
 * 	
 * 	@return One of the following:
 * 		MON_SUCCESS :: Semantic analysis ended correctly and no semantic errors
 * 		encountered.
 * 		MON_ERR_SEMANTIC :: Semantic errors were encountered in the specified AST.
 * 		MON_ERR_NOMEM :: An allocation failure occurred during semantic analysis.
 * 
 * 	@remarks If returned code was MON_SUCCESS, all semantic fields in all AST nodes of
 * 	the specified AST will be properly filled.
 */
MON_PUBLIC Mon_RetCode MON_CALL Mon_SemAnalyse(Mon_Ast* ast, FILE* semErrOutStream);

C_LINKAGE_END

#endif // SEM_H