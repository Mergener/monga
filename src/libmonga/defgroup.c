#include "mon_defgroup.h"

#include <string.h>

#include "mon_debug.h"

void Mon_DefGroupInit(Mon_DefGroup* g) {
    Mon_VectorInit(&g->_defs);
}

Mon_RetCode Mon_DefGroupReg(Mon_DefGroup* g, Mon_AstDef* def) {
    MON_CANT_BE_NULL(g);
    MON_CANT_BE_NULL(def);
    
    //if (Mon_DefGroupFind(g, def->name) != NULL) {
    //    return MON_ERR_DUPLICATE;
    //}

    return Mon_VectorPush(&g->_defs, def);
}

Mon_AstDef* Mon_DefGroupFind(const Mon_DefGroup* g, const char* name) {
    MON_CANT_BE_NULL(g);

    MON_VECTOR_FOREACH(&g->_defs, Mon_AstDef*, def,
        if (strcmp(def->name, name) == 0) {
            return def;
        }
    );
    
    return NULL;
}

void Mon_DefGroupFinalize(Mon_DefGroup* g) {
    if (g == NULL) {
        return;
    }

    Mon_VectorFinalize(&g->_defs);
}