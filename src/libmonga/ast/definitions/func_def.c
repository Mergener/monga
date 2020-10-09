#include "ast/definitions/mon_func_def.h"

#include <assert.h>

#include "mon_alloc.h"
#include "../../strutils.h"

Mon_AstFuncDef* Mon_AstFuncDefNew(const char* funcName,
                                  size_t funcNameLen,
                                  const char* funcRetTypeName,
                                  size_t funcRetTypeLen,
                                  Mon_Vector params,
                                  Mon_AstBlock* body) {

    assert(funcName != NULL);

    Mon_AstFuncDef* ret = Mon_Alloc(sizeof(Mon_AstFuncDef));

    if (ret == NULL) {
        return NULL;
    }

    char* name = DuplicateString(funcName, funcNameLen);
    if (name == NULL) {
        Mon_Free(ret);
        return NULL;
    }

    if (funcRetTypeName == NULL) {
        funcRetTypeName = "void";
        funcRetTypeLen = 4;
    }

    ret->funcRetTypeName = DuplicateString(funcRetTypeName, funcRetTypeLen);
    if (ret->funcRetTypeName == NULL) {
        Mon_Free(ret->funcRetTypeName);
        Mon_Free(ret);
        return NULL;
    }

    ret->funcName = name;
    ret->funcNameLength = funcNameLen;
    ret->parameters = params;
    ret->body = body;

    return ret;
}

void Mon_AstFuncDefDestroy(Mon_AstFuncDef* funcDef, bool rec) {
    if (funcDef == NULL) {
        return;
    }

	if (rec) {
		MON_VECTOR_FOREACH(&funcDef->parameters, Mon_AstParam*, el,
            Mon_AstParamDestroy(el);
        );
	}

    Mon_VectorFinalize(&funcDef->parameters);
	Mon_Free(funcDef->funcName);
	Mon_Free(funcDef);
}