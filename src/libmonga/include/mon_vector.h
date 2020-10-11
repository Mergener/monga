#ifndef MON_VECTOR_H
#define MON_VECTOR_H

#include "mon_defines.h"

#include <stdbool.h>

#include "mon_error.h"

#define MON_VECTOR_FOREACH(vector, type, el, block) \
    {\
        const void** vector_begin_it_ = (vector)->_arr;\
        const void** vector_end_it_ = (vector)->_arr + (vector)->_count;\
        for (const void** it = vector_begin_it_; it != vector_end_it_; ++it) { \
            type el = (type)*it;\
            block \
        }\
    }

C_LINKAGE_BEGIN

typedef struct {

    const void** _arr;
    int          _count;
    int          _cap;

} Mon_Vector;

/**
 *	Initializes a vector. Must be finalized with Mon_VectorFinalize once its use
 *	is finished.
 *	
 *	@param vector The vector to be initialized.
 *	@return One of the following:
 *		MON_SUCCESS :: Vector initialized succesfully.
 *		MON_ERR_NOMEM :: Not enough memory to initialize the vector.
 */
MON_PUBLIC Mon_RetCode MON_CALL Mon_VectorInit(Mon_Vector* vector);

/**
 *  Returns the element at an index of a vector.
 *  
 *  @param vector The vector to fetch the element from.
 *  @param index The vector index to get the element.
 * 
 *  @return The element at the specified index.
 */
MON_PUBLIC void* MON_CALL Mon_VectorGet(const Mon_Vector* vector, int index);

/**
 *  Returns the number of elements in a vector.
 * 
 *  @param vector The vector to get the element count from.
 * 
 *  @return The element count.
 */ 
MON_PUBLIC int MON_CALL Mon_VectorCount(const Mon_Vector* vector);

/**
 * 	Pushes an element onto the end of a vector.
 * 
 * 	@param vector The vector to push to.
 * 	@param obj The object to be pushed onto the vector.
 * 
 *	@return One of the following:
 *		MON_SUCCESS :: Element pushed succesfully.
 *		MON_ERR_NOMEM :: Not enough memory to initialize the vector.
 */ 
MON_PUBLIC Mon_RetCode MON_CALL Mon_VectorPush(Mon_Vector* vector, const void* obj);

/**
 * 	Pops an element from a vector.
 * 
 * 	@param vector The vector to pop from.
 * 	@param ret Pointer to return the popped value to.
 * 
 *	@return One of the following:
 *		MON_SUCCESS :: Element popped succesfully.
 *		MON_ERR_EMPTY_COLLECTION :: The vector was empty.
 */ 
MON_PUBLIC Mon_RetCode MON_CALL Mon_VectorRemove(Mon_Vector* vector, int index);

/**
 * 	Gets the number of elements in a vector.
 * 
 * 	@param vector The vector.
 * 
 *	@return The number of elements in the specified vector.
 */ 
MON_PUBLIC int MON_CALL Mon_VectorGetCount(const Mon_Vector* vector);

/**
 * 	Checks whether a vector is empty.
 * 
 * 	@param vector The vector.
 * 
 *	@return True if the vector is empty, false otherwise.
 */ 
MON_PUBLIC bool MON_CALL Mon_VectorEmpty(const Mon_Vector* vector);

/**
 * 	Clears a vector, removing all its elements.
 * 
 * 	@param vector The vector.
 */ 
MON_PUBLIC void MON_CALL Mon_VectorClear(Mon_Vector* vector);

/**
 *	Finalizes a vector, cleaning up any resources used by it.
 *
 *  @param vector The vector to be finalized.
 */ 
MON_PUBLIC void MON_CALL Mon_VectorFinalize(Mon_Vector* vector);

C_LINKAGE_END

#endif // MON_VECTOR_H