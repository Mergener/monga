#ifndef MON_COND_H
#define MON_COND_H

#include <mon_defines.h>

#include <stdbool.h>

#include "ast/conditions/mon_bincond.h"
#include "ast/conditions/mon_comparison.h"

C_LINKAGE_BEGIN

typedef enum {

	MON_COND_BIN,
	MON_COND_COMPARISON

} Mon_AstCondKind;

typedef struct Mon_AstCond_ {

	union {
		
		/** Available if condKind == MON_COND_BIN */
		Mon_AstBinCond*    binCond;

		/** Available if condKind == MON_COND_COMPARISON */
		Mon_AstComparison* compar;

	} condition;

	Mon_AstCondKind condKind;

	/** If true, the condition is marked as negated. */
	bool negate;

} Mon_AstCond;

MON_PUBLIC Mon_AstCond* MON_CALL Mon_AstCondNewBin(Mon_AstBinCond* binCond, Mon_AstBinCond* bin);
MON_PUBLIC Mon_AstCond* MON_CALL Mon_AstCondNewCompar(Mon_AstBinCond* binCond, Mon_AstComparison* compar);
MON_PUBLIC void MON_CALL Mon_AstCondDestroy(Mon_AstCond* node, bool rec);

C_LINKAGE_END

#endif // MON_COND_H