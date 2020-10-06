#include "ast/statements/mon_call.h"

#include "mon_alloc.h"
#include "../strutils.h"

Mon_AstCall* Mon_AstCallNew(const char* funcName,
                            size_t funcNameLen,
							Mon_AstExp* firstParameter) {
	Mon_AstCall* ret = Mon_Alloc(sizeof(Mon_AstCall));

	if (ret == NULL) {
		return NULL;
	}

	ret->funcName = DuplicateString(funcName, funcNameLen);
	if (ret->funcName == NULL) {
		Mon_Free(ret);
		return NULL;
	}

	ret->funcNameLen = funcNameLen;
	ret->firstParameter = firstParameter;

	return ret;
}

void Mon_AstCallDestroy(Mon_AstCall* node, bool rec) {
	if (rec) {
		Mon_AstExpDestroy(node->firstParameter, true);
	}

	Mon_Free(node->funcName);
	Mon_Free(node);
}