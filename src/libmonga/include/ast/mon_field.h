#ifndef MON_FIELD_H
#define MON_FIELD_H

#include <mon_defines.h>

C_LINKAGE_BEGIN

typedef struct {

	char* typeName;
	char* fieldName;

} Mon_AstField;

MON_PUBLIC Mon_AstField* MON_CALL Mon_AstFieldNew(const char* typeName, const char* fieldName);
MON_PUBLIC void MON_CALL Mon_AstFieldDestroy(Mon_AstField* node);

C_LINKAGE_END

#endif // MON_FIELD_H