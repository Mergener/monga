#ifndef MON_AST_BASE_H
#define MON_AST_BASE_H

#include <mon_defines.h>

C_LINKAGE_BEGIN

typedef struct {

	int line;
	int column;

} Mon_AstNodeHeader;

C_LINKAGE_END

#endif // MON_AST_BASE_H