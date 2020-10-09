#include "ast/mon_call.h"

#include <assert.h>
#include "mon_alloc.h"
#include "../strutils.h"

Mon_AstCall* Mon_AstCallNew(const char* funcName,
                            size_t funcNameLen,
							Mon_Vector parameters) {
	assert(funcName != NULL);

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
	ret->parameterList = parameters;

	return ret;
}

void Mon_AstCallDestroy(Mon_AstCall* node, bool rec) {
	if (node == NULL) {
		return;
	}

	if (rec) {
		MON_VECTOR_FOREACH(&node->parameterList, Mon_AstExp*, param,
			Mon_AstExpDestroy(param, true);
		);
	}

	Mon_VectorFinalize(&node->parameterList);
	Mon_Free(node->funcName);
	Mon_Free(node);
}