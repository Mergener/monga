#include "symbol.h"

const char* GetSymbolName(const Symbol* s) {
	MON_CANT_BE_NULL(s);

	switch (s->kind) {
		case SYM_PARAM:
			return s->definition.param->name;

		case SYM_FUNC:
			return s->definition.func->funcName;

		case SYM_VAR:
			return s->definition.var->varName;

		case SYM_TYPE:
			return s->definition.type->typeName;

		default:
			MON_ASSERT(false, "Unimplemented symbol kind. (got %d)", (int)s->kind);
			return "";
	}
}