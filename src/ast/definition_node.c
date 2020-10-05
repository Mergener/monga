#include "ast/definition_node.h"

#include "alloc.h"

Mon_AstDef* Mon_AstVarDefNew(const char* varName,
                             size_t varNameLen) {

	Mon_AstDef* ret = Mon_Alloc(sizeof(Mon_AstDef));

	if (ret == NULL) {
		return NULL;
	}

	ret->defKind = MON_AST_DEF_VAR;
	ret->definition.function.funcName = varName;
	ret->definition.function.funcNameLength = varNameLen;
	ret->next = NULL;

	return ret;
}

Mon_AstDef* Mon_AstFuncDefNew(const char* funcName,
                              size_t funcNameLen) {

	Mon_AstDef* ret = Mon_Alloc(sizeof(Mon_AstDef));

	if (ret == NULL) {
		return NULL;
	}

	ret->defKind = MON_AST_DEF_FUNC;
	ret->definition.function.funcName = funcName;
	ret->definition.function.funcNameLength = funcNameLen;
	ret->next = NULL;

	return ret;
}
