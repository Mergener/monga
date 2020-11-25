#ifndef MON_VECTOR_H
#define MON_VECTOR_H

#include "mon_defines.h"

#include <stdbool.h>

#include "mon_error.h"

C_LINKAGE_BEGIN

typedef struct Mon_Vector_ Mon_Vector;

/**
 *  Initializes a vector. Must be finalized with Mon_VectorFinalize once its use
 *  is finished.
 *  
 *  @param vector The vector to be initialized.
 *  @return One of the following:
 *   MON_SUCCESS :: Vector initialized succesfully.
 *   MON_ERR_NOMEM :: Not enough memory to initialize the vector.
 */
MON_PUBLIC Mon_RetCode MON_CALL Mon_VectorInit(Mon_Vector* vector);

/**
 *  Finds the index of an element inside a vector.
 * 
 *  @param vector The vector to search the element for.
 *  @param elem The element to get the index of.
 * 
 *  @returns The element index or -1 if the element is not found inside the vector.
 */
MON_PUBLIC int MON_CALL Mon_VectorGetIndex(const Mon_Vector* vector, const void* elem);

/**
 *  Tells whether an element is inside a vector or not.
 *  
 *  @param vector The vector to search the element for.
 *  @param elem The element to be checked.
 * 
 *  @returns True if the vector contains the element, false otherwise.
 */
MON_PUBLIC bool MON_CALL Mon_VectorContains(const Mon_Vector* vector, const void* elem);

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
 *  Pushes an element onto the end of a vector.
 * 
 *  @param vector The vector to push to.
 *  @param obj The object to be pushed onto the vector.
 * 
 *  @return One of the following:
 *   MON_SUCCESS :: Element pushed succesfully.
 *   MON_ERR_NOMEM :: Not enough memory to initialize the vector.
 */ 
MON_PUBLIC Mon_RetCode MON_CALL Mon_VectorPush(Mon_Vector* vector, const void* obj);

/**
 *  Removes an element from a vector.
 * 
 *  @param vector The non-empty vector to remove an element from.
 */ 
MON_PUBLIC void MON_CALL Mon_VectorRemove(Mon_Vector* vector, int index);

/**
 *  Removes the last element of the vector.
 * 
 *  @param vector The non-empty vector to remove an element from.
 */
MON_PUBLIC void MON_CALL Mon_VectorRemoveLast(Mon_Vector* vector);

/**
 *  Checks whether a vector is empty.
 * 
 *  @param vector The vector.
 * 
 *  @return True if the vector is empty, false otherwise.
 */ 
MON_PUBLIC bool MON_CALL Mon_VectorEmpty(const Mon_Vector* vector);

/**
 *  Clears a vector, removing all its elements.
 * 
 *  @param vector The vector.
 */ 
MON_PUBLIC void MON_CALL Mon_VectorClear(Mon_Vector* vector);

/**
 *  Transfers the ownership of a vector's internal buffer and resets it.
 *  After claiming the ownership of the vector's buffer, the vector will
 *  be reset, containing no more elements.
 * 
 *  @param vector The vector.
 *  @param outPtr A pointer to the variable that will receive the address
 *  of the vector's internal buffer.
 *  @param count A pointer to the variable that will receive the number
 *  of elements contained in the buffer. Ignored if NULL.
 *  @param capacity A pointer to the variable that will receive the
 *  storage capacity of elements in the buffer. Ignored if NULL.
 * 
 *  @remarks Since the internal buffer ownership is transferred, it is the
 *  caller's responsibility to release its memory when necessary.
 *  The internal buffer can be freed with Mon_Free. Be aware of issues that
 *  could happen if custom allocators are being used. For that, it is suggested
 *  to read the custom allocators documentation in 'mon_alloc.h'.
 */
MON_PUBLIC void MON_CALL Mon_VectorClaim(Mon_Vector* vector, void** outPtr, int* count, int* capacity);

/**
 *  Finalizes a vector, cleaning up any resources used by it.
 *
 *  @param vector The vector to be finalized.
 * 
 *  @remarks Calling this on a non-initialized may trigger undefined behavior.
 *  However, this function can safely be called on a vector in which Mon_VectorInit was
 *  called and failed or even on a vector that has already been finalized.
 */ 
MON_PUBLIC void MON_CALL Mon_VectorFinalize(Mon_Vector* vector);

/**
 *  Utility macro to iterate over vectors.
 * 
 *  @param vector A pointer to the vector to iterate on.
 *  @param type A type to cast each element of the vector to (like const int*).
 *  @param el A name to refer to each element of the vector.
 *  @param block A block of code to execute upon each element of the vector.
 * 
 *  @remarks Usage example: Print all strings in a vector.
 *  
 *  void foo(Mon_Vector* vecOfStrings) {
 *      MON_VECTOR_FOREACH(vecOfStrings, const char*, s, 
 *          printf("%s\n", s);
 *      );
 *  }
 */
#define MON_VECTOR_FOREACH(vector, type, el, block) \
    do {\
        const Mon_Vector* const target_vector_ = (vector); \
        const void** vector_begin_it_ = (target_vector_)->_arr;\
        const void** vector_end_it_ = (target_vector_)->_arr + (target_vector_)->_count;\
        for (const void** it_ = vector_begin_it_; it_ != vector_end_it_; ++it_) { \
            type el = (type)*it_;\
            block \
        }\
    } while(false)

struct Mon_Vector_ {
    const void** _arr;
    int          _count;
    int          _cap;
};

C_LINKAGE_END

#endif // MON_VECTOR_H