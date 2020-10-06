#ifndef MON_BINOP_H
#define MON_BINOP_H

#include <mon_defines.h>

typedef struct Mon_AstExp_ Mon_AstExp;

C_LINKAGE_BEGIN

/** A binary operation type. */
typedef enum {

	MON_BINOP_ADD,
	MON_BINOP_SUB,
	MON_BINOP_MUL,
	MON_BINOP_DIV,
	MON_BINOP_MODULO 

} Mon_BinOpType;

typedef struct {

	Mon_AstExp* left;
	Mon_AstExp* right;
	
	Mon_BinOpType op;

} Mon_AstBinOp;

/**
 * 	Creates and return a binary operation node.
 * 
 * 	@param l The expression to the left of the operation node.
 * 	@param r The expression to the right of the operation node.
 * 	@param op The operation type of the node.
 * 
 * 	@returns A pointer to the created node or NULL if allocation failed.
 */ 
MON_PUBLIC Mon_AstBinOp* MON_CALL Mon_AstBinOpNew(Mon_AstExp* l, Mon_AstExp* r, Mon_BinOpType op);

/**
 * 	Deallocates a binary operation node created with Mon_AstBinOpNew.
 */
MON_PUBLIC void MON_CALL Mon_AstBinOpDestroy(Mon_AstBinOp* node);

C_LINKAGE_END

#endif // MON_BINOP_H