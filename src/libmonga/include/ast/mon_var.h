#ifndef MON_VAR_H
#define MON_VAR_H

#include <mon_defines.h>

#include "ast/mon_exp.h"

C_LINKAGE_BEGIN

typedef struct Mon_AstVar_ Mon_AstVar;

typedef enum {

	MON_VAR_DIRECT,
	MON_VAR_FIELD,
	MON_VAR_INDEXED

} Mon_AstVarKind;

typedef struct Mon_AstVar_ {

	Mon_AstVarKind varKind;

	union {
		struct {
			Mon_AstExp* expr;
			char* fieldName;
		} field;

		struct {
			Mon_AstExp* indexedExpr;
			Mon_AstExp* indexExpr;
		} indexed;

		char* direct;
	} var;

} Mon_AstVar;

MON_PUBLIC Mon_AstVar* MON_CALL Mon_AstVarNewIndexed(Mon_AstExp* indexedExpr, Mon_AstExp* indexExpr);
MON_PUBLIC Mon_AstVar* MON_CALL Mon_AstVarNewField(Mon_AstExp* expr, const char* fieldName);
MON_PUBLIC Mon_AstVar* MON_CALL Mon_AstVarNewDirect(const char* varName);

MON_PUBLIC void MON_CALL Mon_AstVarDestroy(Mon_AstVar* node, bool rec);

C_LINKAGE_END

#endif // MON_VAR_H