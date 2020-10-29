#include "ast/mon_var.h"

#include <string.h>
#include <mon_debug.h>

#include "mon_alloc.h"
#include "../strutils.h"
#include "../sem/types.h"

Mon_AstVar* Mon_AstVarNewIndexed(Mon_AstExp* indexedExpr, Mon_AstExp* indexExpr) {
    MON_CANT_BE_NULL(indexedExpr);
    MON_CANT_BE_NULL(indexExpr);

    Mon_AstVar* ret = Mon_Alloc(sizeof(Mon_AstVar));
    if (ret == NULL) {
        return NULL;
    }

    ret->varKind = MON_VAR_INDEXED;
    ret->var.indexed.indexedExpr = indexedExpr;
    ret->var.indexed.indexExpr = indexExpr;

    ret->semantic.type = NULL;

    return ret;
}

Mon_AstVar* Mon_AstVarNewField(Mon_AstExp* expr, const char* fieldName) {
    MON_CANT_BE_NULL(expr);

    Mon_AstVar* ret = Mon_Alloc(sizeof(Mon_AstVar));
    if (ret == NULL) {
        return NULL;
    }

    ret->var.field.fieldName = DuplicateString(fieldName, strlen(fieldName));
    if (ret->var.field.fieldName == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    ret->varKind = MON_VAR_FIELD;
    ret->var.field.expr = expr;

    ret->semantic.type = NULL;
    ret->var.field.semantic.field = NULL;

    return ret;
}

Mon_AstVar* Mon_AstVarNewDirect(const char* varName) {
    MON_CANT_BE_NULL(varName);

    Mon_AstVar* ret = Mon_Alloc(sizeof(Mon_AstVar));
    if (ret == NULL) {
        return NULL;
    }

    ret->var.direct.name = DuplicateString(varName, strlen(varName));
    if (ret->var.direct.name == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    ret->varKind = MON_VAR_DIRECT;
    ret->semantic.type = NULL;
    ret->var.direct.semantic.definitionKind = MON_SEM_VARDEF_VAR;
    ret->var.direct.semantic.definition.varDef = NULL;

    return ret;
}

void Mon_AstVarDestroy(Mon_AstVar* node, bool rec) {
    if (node == NULL) {
        return;
    }

    switch (node->varKind) {
        case MON_VAR_FIELD:
            if (rec) {
                Mon_AstExpDestroy(node->var.field.expr, true);
            }
            Mon_Free(node->var.field.fieldName);
            break;

        case MON_VAR_INDEXED:
            if (rec) {
                Mon_AstExpDestroy(node->var.indexed.indexedExpr, true);
                Mon_AstExpDestroy(node->var.indexed.indexExpr, true);
            }
            break;

        case MON_VAR_DIRECT:
            Mon_Free(node->var.direct.name);
            break;

        default:
            MON_ASSERT(false, "Unimplemented varKind. (got %d)", (int)node->varKind);
            break;
    }

    Mon_Free(node);
}