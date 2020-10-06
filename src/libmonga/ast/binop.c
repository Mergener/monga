#include "ast/mon_binop.h"

#include "mon_alloc.h"

Mon_AstBinOp* Mon_AstBinOpNew(Mon_AstExp* l, Mon_AstExp* r, Mon_BinOpType op) {
	Mon_AstBinOp* ret = Mon_Alloc(sizeof(Mon_AstBinOp));

	if (ret == NULL) {
		return NULL;
	}

	ret->left = l;
	ret->right = r;
	ret->op = op;

	return ret;
}

void Mon_AstBinOpDestroy(Mon_AstBinOp* node) {
	Mon_Free(node);
}