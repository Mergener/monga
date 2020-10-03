#include "monstack.h"

#include "alloc.h"

#define DEFAULT_SIZE 16
#define MIN_SIZE DEFAULT_SIZE

Mon_RetCode Mon_StackInit(Mon_Stack* stack) {

	stack->_cap = DEFAULT_SIZE * sizeof(void*);
	stack->_arr = Mon_Alloc(stack->_cap);
	stack->_count = 0;

	if (stack->_arr == NULL) {
		return MON_ERR_NOMEM;
	}

	return MON_SUCCESS;
}

Mon_RetCode Mon_StackPush(Mon_Stack* stack, const void* obj) {
	if (stack->_count == stack->_cap) {
		int newCap = stack->_cap * 2;
		void** newMem = Mon_Realloc(stack->_arr, newCap);

		if (newMem == NULL) {
			return MON_ERR_NOMEM;
		}

		stack->_arr = newMem;
		stack->_cap = newCap;
	}

	stack->_arr[stack->_count] = obj;
	stack->_count++;

	return MON_SUCCESS;
}

Mon_RetCode Mon_StackPop(Mon_Stack* stack, void** ret) {
	if (stack->_cap > MIN_SIZE && stack->_count <= stack->_cap/3) {
		// In pop operation, it is not strictly necessary to resize the array.
		// So, if reallocation fails, we just keep the original one.

		int halfCap = stack->_cap/2;
		int newCap = halfCap > MIN_SIZE ? halfCap : MIN_SIZE;
		void** newMem = Mon_Realloc(stack->_arr, newCap);

		if (newMem != NULL) {
			stack->_arr = newMem;
			stack->_cap = newCap;
		}
	}

	Mon_RetCode ret = Mon_StackPeek(stack, ret);
	stack->_count--;
	return ret;
}

Mon_RetCode Mon_StackPeek(const Mon_Stack* stack, void** ret) {
	if (stack->_count <= 0) {
		return MON_ERR_EMPTY_COLLECTION;
	}

	*ret = stack->_arr[stack->_count - 1];
}

int Mon_StackGetCount(const Mon_Stack* stack) {
	return stack->_count;
}

bool Mon_StackEmpty(const Mon_Stack* stack) {
	return stack->_count == 0;
}

MON_API void MON_CALL Mon_StackClear(Mon_Stack* stack) {
	stack->_count = 0;
}

void Mon_StackFinalize(Mon_Stack* stack) {
	stack->_cap = 0;
	stack->_count = 0;
	stack->_arr = NULL;

	Mon_Free(stack->_arr);
}