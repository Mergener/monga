#ifndef MON_DEFGROUP_H
#define MON_DEFGROUP_H

#include <mon_defines.h>

#include "ast/mon_definition.h"
#include "mon_vector.h"

C_LINKAGE_BEGIN

typedef struct {

    Mon_Vector _defs;

} Mon_DefGroup;

MON_PUBLIC void MON_CALL Mon_DefGroupInit(Mon_DefGroup* g);
MON_PUBLIC Mon_RetCode MON_CALL Mon_DefGroupReg(Mon_DefGroup* g, Mon_AstDef* def);
MON_PUBLIC Mon_AstDef* MON_CALL Mon_DefGroupFind(const Mon_DefGroup* g, const char* name);
MON_PUBLIC void MON_CALL Mon_DefGroupFinalize(Mon_DefGroup* g);

#define MON_DEFGROUP_FOREACH(defGroup, defName, block) \
    MON_VECTOR_FOREACH(&(defGroup)->_defs, Mon_AstDef*, defName, block)

C_LINKAGE_END

#endif // MON_DEFGROUP_H