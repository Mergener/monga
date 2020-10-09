#include "ast/mon_block.h"

#include "mon_alloc.h"
#include "ast/definitions/mon_var_def.h"

Mon_AstBlock* Mon_AstBlockNew(Mon_Vector varDefs, Mon_Vector statements) {
	Mon_AstBlock* ret = Mon_Alloc(sizeof(Mon_AstBlock));
	if (ret == NULL) {
		return NULL;
	}

	ret->varDefs = varDefs;
	ret->statements = statements;

	return ret;
}

void Mon_AstBlockDestroy(Mon_AstBlock* node, bool rec) {
	if (node == NULL) {
		return;
	}

	if (rec) {
		MON_VECTOR_FOREACH(&node->statements, Mon_AstStatement*, stmt,
			Mon_AstStatementDestroy(stmt, true);
		);
		MON_VECTOR_FOREACH(&node->varDefs, Mon_AstVarDef*, var,
			Mon_AstVarDefDestroy(var);
		);
	}

	Mon_VectorFinalize(&node->varDefs);
	Mon_VectorFinalize(&node->statements);
	Mon_Free(node);
}