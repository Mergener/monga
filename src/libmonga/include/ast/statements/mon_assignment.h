#ifndef MON_ASSIGNMENT_H
#define MON_ASSIGNMENT_H

#include <mon_defines.h>

#include "ast/mon_exp.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstLvalue_ Mon_AstLvalue;

typedef struct {

	Mon_AstLvalue* lvalue;
	Mon_AstExp*    rvalueExp;

} Mon_AstAssignment;

MON_PUBLIC Mon_AstAssignment* MON_CALL Mon_AstAssignmentNew(Mon_AstLvalue* lvalue, Mon_AstExp* exp);
MON_PUBLIC void MON_CALL Mon_AstAssignmentDestroy(Mon_AstAssignment* node, bool rec);

C_LINKAGE_END

#endif // MON_ASSIGNMENT_H