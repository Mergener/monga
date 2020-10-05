#ifndef PARSER_H
#define PARSER_H

#include "defines.h"

#include <stdio.h>

#include "ast.h"
#include "error.h"

/**
 *	Parses the specified input stream that contains Monga code and generates 
 *	an AST (Abstract Syntax Tree) for it.
 *
 *	@param f The input stream to read from. Use C's stdin to read from the standard
 *	input.
 *	@param outAst A pointer to an AST object that will have the AST data written upon.
 *	@return One of the following return codes:
 *		MON_SUCCESS       :: Parsing finished gracefully.
 *		MON_ERR_NOMEM     :: Parsing failed due to lack of available memory.
 *		MON_ERR_BAD_ARG   :: Either 'f' or 'outAst' (or both) were NULL.
 *
 *	@remarks If an ongoing parse is being executed, this call will block the calling
 *	thread to wait until it is terminated.
 *
 */
MON_API Mon_RetCode MON_CALL Mon_Parse(FILE* f, Mon_Ast* outAst);

#endif // PARSER_H