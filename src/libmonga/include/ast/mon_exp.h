#ifndef MON_EXP_H
#define MON_EXP_H

#include <mon_defines.h>

#include "mon_binop.h"

C_LINKAGE_BEGIN

typedef enum {

	MON_EXP_BINOP

} Mon_AstExpType;

typedef struct Mon_AstExp_ {

	union {
		Mon_AstBinOp* binaryOperation;
	} exp;

	Mon_AstExpType expType;

	/**
	 *	In cases where this node belongs to a linked list of expressions,
	 *	its 'next' pointer points to the next node or NULL if
	 *	this node is the last in the list. 
	 *
	 * 	A case for a list of expressions is a call's parameter list.
	 */
	struct Mon_AstExp_* next;

} Mon_AstExp;

C_LINKAGE_END

#endif // MON_EXP_H