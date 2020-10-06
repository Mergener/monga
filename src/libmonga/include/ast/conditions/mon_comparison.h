#ifndef MON_COMPARISON_H
#define MON_COMPARISON_H

#include <mon_defines.h>

#include "ast/mon_exp.h"

C_LINKAGE_BEGIN

typedef enum {

	MON_CMP_EQ,
	MON_CMP_NE,
	MON_CMP_GT,
	MON_CMP_GE,
	MON_CMP_LT,
	MON_CMP_LE

} Mon_AstComparisonType;

typedef struct {

	Mon_AstExp* left;
	Mon_AstExp* right;
	Mon_AstComparisonType comparisonType;

} Mon_AstComparison;

MON_PUBLIC Mon_AstComparison* MON_CALL Mon_AstComparisonNew(Mon_AstExp* l, Mon_AstExp* r, Mon_AstComparisonType ct);
MON_PUBLIC void MON_CALL Mon_AstComparisonDestroy(Mon_AstComparison* node, bool rec);

C_LINKAGE_END

#endif // MON_COMPARISON_H