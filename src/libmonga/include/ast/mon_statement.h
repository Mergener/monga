#ifndef MON_STATEMENT_H
#define MON_STATEMENT_H

#include <mon_defines.h>

C_LINKAGE_BEGIN

typedef enum {

	MON_STMT_ASSIGNMENT

} Mon_StmtType;

typedef struct {


	Mon_AstStatement* next;
	
} Mon_AstStatement;

C_LINKAGE_END

#endif // MON_STATEMENT_H