#ifndef SEM_H
#define SEM_H

#include <mon_defines.h>

#include <stdbool.h>

#include "mon_ast.h"

/**
 * 	Performs semantic analysis on the specified abstract syntax tree,
 * 	filling semantic fields in each of its nodes.
 * 
 * 	@param ast The AST to analyse.
 * 	
 * 	@return True if no semantic errors were found,
 * 	false otherwise.
 */
MON_PRIVATE bool PerformSemanticAnalysis(Mon_Ast* ast);

#endif // SEM_H