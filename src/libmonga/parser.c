#include "mon_parser.h"

#include <stdatomic.h>
#include <stdbool.h>

#include "grammar.tab.h"

#include "mon_error.h"
#include "lex_private.h"

extern FILE* yyin;

/**
 *  The user provided AST object to be filled.
 *  
 *  Set by Mon_Parse procedure.
 */
Mon_Ast* mon_TargetAst;

/**
 * True if the user wants to dump reduction rules when parsing.
 */
bool mon_DumpReduces = false;

Mon_Vector* mon_ParseStack = NULL;

static atomic_bool s_Busy = false;

Mon_RetCode Mon_Parse(FILE* f, Mon_Ast* outAst, Mon_ParseFlags flags) {
    if (f == NULL || outAst == NULL) {
        return MON_ERR_BAD_ARG;
    }

    // Wait until an ongoing parsing is finished.
    while (s_Busy);
    s_Busy = true;

    PrepareLex();

    Mon_Vector stack;
    if (Mon_VectorInit(&stack) != MON_SUCCESS) {
        return MON_ERR_NOMEM;
    }

    mon_TargetAst = outAst;
    mon_DumpReduces = (flags & MON_PARSEFLAGS_DUMPREDUCES) != 0;

    yyin = f;

    Mon_RetCode retCode;

    switch (yyparse()) {
        case 0:
            retCode = MON_SUCCESS;
            outAst->astState = MON_ASTSTATE_SYNTAX_OK;
            break;

        case 1:
            retCode = MON_ERR_SYNTAX;
            outAst->astState = MON_ASTSTATE_NONE;
            break;

        case 2:
            retCode = MON_ERR_NOMEM;
            outAst->astState = MON_ASTSTATE_NONE;
            break;
    }

    s_Busy = false;

    Mon_VectorFinalize(&stack);

    return retCode;
}