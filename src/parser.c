#include "parser.h"

#include <stdatomic.h>
#include <stdbool.h>

#include "grammar.tab.h"

#include "error.h"

/**
 *	The user provided AST object to be filled.
 *	
 *	Set by Mon_Parse procedure.
 */
Mon_Ast* mon_TargetAst;

static atomic_bool s_Busy = false;

Mon_RetCode Mon_Parse(FILE* f, Mon_Ast* outAst) {
	if (f == NULL || outAst == NULL) {
		return MON_ERR_BAD_ARG;
	}

	// Wait until an ongoing parsing is finished.
	while (s_Busy);
	s_Busy = true;

	mon_TargetAst = outAst;

	Mon_RetCode retCode;

	switch (yyparse()) {
		case 0:
			retCode = MON_SUCCESS;
			break;

		case 1:
			retCode = MON_ERR_SYNTAX;
			break;

		case 2:
			retCode = MON_ERR_NOMEM;
			break;
	}

	s_Busy = false;

	return retCode;
}