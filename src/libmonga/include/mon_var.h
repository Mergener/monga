#ifndef MON_VAR_H
#define MON_VAR_H

#include <mon_defines.h>

#include <stdlib.h>

C_LINKAGE_BEGIN

/**
 * 	A variable reference node. Not to be confused with a variable
 * 	declaration node (Mon_AstVarDef).
 */
typedef struct {

	const char* varName;
	size_t varNameLen;

	/**
	 *	Pointer to this variable's field being acessed. Generally NULL.
	 *
	 * 	Example:
	 * 	
	 * 		my2dVector.X :: This will generate two var nodes, one for
	 * 		my2dVector and another for X. In my2dVector, the accessedField
	 * 		will point to the X node, while X's accessedField pointer will
	 * 		be NULL, since there's no field of X being accessed.
	 */
	Mon_AstVar* accessedField;

} Mon_AstVar;

MON_PUBLIC Mon_AstVar* MON_CALL Mon_AstVarNew(const char* varName, size_t varNameLen);
MON_PUBLIC void MON_CALL Mon_AstVarDestroy(Mon_AstVar* node, bool rec);

C_LINKAGE_END

#endif // MON_VAR_H