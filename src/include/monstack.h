#ifndef MON_STACK_H
#define MON_STACK_H

#include "defines.h"

#include <stdbool.h>

#include "error.h"

C_LINKAGE_BEGIN

typedef struct {

	const void** _arr;
	int          _count;
	int          _cap;

} Mon_Stack;

/**
 *	Initializes a stack. Must be finalized with Mon_StackFinalize once its use
 *	is finished.
 *	
 *	@param stack The stack to be initialized.
 *	@return One of the following:
 *		MON_SUCCESS :: Stack initialized succesfully.
 *		MON_ERR_NOMEM :: Not enough memory to initialize the stack.
 */
MON_PUBLIC Mon_RetCode MON_CALL Mon_StackInit(Mon_Stack* stack);

/**
 * 	Pushes an element onto a stack.
 * 
 * 	@param stack The stack to push to.
 * 	@param obj The object to be pushed onto the stack.
 * 
 *	@return One of the following:
 *		MON_SUCCESS :: Element pushed succesfully.
 *		MON_ERR_NOMEM :: Not enough memory to initialize the stack.
 */ 
MON_PUBLIC Mon_RetCode MON_CALL Mon_StackPush(Mon_Stack* stack, const void* obj);

/**
 * 	Pops an element from a stack.
 * 
 * 	@param stack The stack to pop from.
 * 	@param ret Pointer to return the popped value to.
 * 
 *	@return One of the following:
 *		MON_SUCCESS :: Element popped succesfully.
 *		MON_ERR_EMPTY_COLLECTION :: The stack was empty.
 */ 
MON_PUBLIC Mon_RetCode MON_CALL Mon_StackPop(Mon_Stack* stack, const void** ret);

/**
 * 	Peeks at an element in a stack.
 * 
 * 	@param stack The stack to peek at.
 * 	@param ret Pointer to return the peeked value to.
 * 
 *	@return One of the following:
 *		MON_SUCCESS :: Element peeked succesfully.
 *		MON_ERR_EMPTY_COLLECTION :: The stack was empty.
 */ 
MON_PUBLIC Mon_RetCode MON_CALL Mon_StackPeek(const Mon_Stack* stack, const void** ret);

/**
 * 	Gets the number of elements in a stack.
 * 
 * 	@param stack The stack.
 * 
 *	@return The number of elements in the specified stack.
 */ 
MON_PUBLIC int MON_CALL Mon_StackGetCount(const Mon_Stack* stack);

/**
 * 	Checks whether a stack is empty.
 * 
 * 	@param stack The stack.
 * 
 *	@return True if the stack is empty, false otherwise.
 */ 
MON_PUBLIC bool MON_CALL Mon_StackEmpty(const Mon_Stack* stack);

/**
 * 	Clears a stack, removing all its elements.
 * 
 * 	@param stack The stack.
 */ 
MON_PUBLIC void MON_CALL Mon_StackClear(Mon_Stack* stack);

/**
 *	Finalizes a stack, cleaning up any resources used by it.
 */ 
MON_PUBLIC void MON_CALL Mon_StackFinalize(Mon_Stack* stack);

C_LINKAGE_END

#endif // MON_STACK_H