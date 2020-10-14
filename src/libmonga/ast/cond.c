#include "ast/mon_cond.h"

#include <mon_debug.h>

#include "mon_alloc.h"
#include "ast/mon_exp.h"

Mon_AstCond* Mon_AstCondNewBin(Mon_AstCond* l, Mon_AstCond* r, Mon_BinCondKind condKind) {
    MON_CANT_BE_NULL(l);
    MON_CANT_BE_NULL(r);    

    Mon_AstCond* ret = Mon_Alloc(sizeof(*ret));
    if (ret == NULL) {
        return NULL;
    }

    ret->condKind = MON_COND_BIN;
    ret->condition.binCond.left = l;
    ret->condition.binCond.right = r;
    ret->condition.binCond.binCondKind = condKind;
    ret->negate = false;

    return ret;
}

Mon_AstCond* Mon_AstCondNewCompar(Mon_AstExp* l, Mon_AstExp* r, Mon_ComparKind comparKind) {
    MON_CANT_BE_NULL(l);
    MON_CANT_BE_NULL(r);    

    Mon_AstCond* ret = Mon_Alloc(sizeof(*ret));
    if (ret == NULL) {
        return NULL;
    }

    ret->condKind = MON_COND_COMPARISON;
    ret->condition.compar.left = l;
    ret->condition.compar.right = r;
    ret->condition.compar.comparKind = comparKind;
    ret->negate = false;

    return ret;
}

void Mon_AstCondDestroy(Mon_AstCond* node, bool rec) {
    if (node == NULL) {
        return;
    }

    // Allows tail call optimization
    Mon_AstCond cond = *node;

    Mon_Free(node);

    if (rec) {
        switch (cond.condKind) {
            case MON_COND_BIN:
                Mon_AstCondDestroy(cond.condition.binCond.left, true);
                Mon_AstCondDestroy(cond.condition.binCond.right, true);
                break;
            
            case MON_COND_COMPARISON:
                Mon_AstExpDestroy(cond.condition.compar.left, true);
                Mon_AstExpDestroy(cond.condition.compar.right, true);
                break;

            default:
                MON_ASSERT(false, "Unimplemented condition kind. (got %d)", (int)cond.condKind);
                break;
        }
    }
}