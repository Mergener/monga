#include "ast/mon_var.h"

#include <string.h>
#include <assert.h>

#include "mon_alloc.h"
#include "../strutils.h"

Mon_AstVar* Mon_AstVarNewIndexed(Mon_AstExp* indexedExpr, Mon_AstExp* indexExpr) {
	assert(indexedExpr != NULL);
	assert(indexExpr != NULL);

	Mon_AstVar* ret = Mon_Alloc(sizeof(Mon_AstVar));
	if (ret == NULL) {
		return NULL;
	}

	ret->varKind = MON_VAR_INDEXED;
	ret->var.indexed.indexedExpr = indexedExpr;
	ret->var.indexed.indexExpr = indexExpr;

	return ret;
}

Mon_AstVar* Mon_AstVarNewField(Mon_AstExp* expr, const char* fieldName) {
	assert(expr != NULL);

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

	return ret;
}

Mon_AstVar* Mon_AstVarNewDirect(const char* varName) {
	assert(varName != NULL);

	Mon_AstVar* ret = Mon_Alloc(sizeof(Mon_AstVar));
	if (ret == NULL) {
		return NULL;
	}

	ret->var.direct = DuplicateString(varName, strlen(varName));
	if (ret->var.direct == NULL) {
		Mon_Free(ret);
		return NULL;
	}

	ret->varKind = MON_VAR_DIRECT;

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
			Mon_Free(node->var.direct);
			break;

		default:
			assert(false);
			break;
	}
}