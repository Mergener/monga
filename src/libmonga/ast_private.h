#ifndef AST_PRIVATE_H
#define AST_PRIVATE_H

#include <mon_defines.h>

#include "mon_ast.h"

struct Mon_Ast_ {
    /** Vector containing all program definitions. Stored elements are of type Mon_AstDef*. */
    Mon_DefGroup definitions;

    Mon_AstState astState;

    char* moduleName;

    struct {
        /** Contains the list types used by this module. */
        Mon_Vector usedTypes;

        bool hasEntryPointFunction;
    } semantic;
};

#endif // AST_PRIVATE_H