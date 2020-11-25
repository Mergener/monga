#include "mon_vector.h"

#include <mon_debug.h>

#include "mon_alloc.h"

#define DEFAULT_CAP 16
#define MIN_SIZE DEFAULT_CAP

Mon_RetCode Mon_VectorInit(Mon_Vector* vector) {
    MON_CANT_BE_NULL(vector);

    vector->_arr = Mon_Alloc(DEFAULT_CAP * sizeof(void*));
    vector->_count = 0;

    if (vector->_arr == NULL) {
        vector->_cap = 0;
        return MON_ERR_NOMEM;
    }

    vector->_cap = DEFAULT_CAP;

    return MON_SUCCESS;
}

void* Mon_VectorGet(const Mon_Vector* vector, int index) {
    MON_CANT_BE_NULL(vector);
    MON_ASSERT(((unsigned int)index) < (unsigned int)vector->_count, 
        "Specified index (%d) cannot be >= vector element count (%d).",
        index, vector->_count);

    return (void*)vector->_arr[index];
}

int Mon_VectorCount(const Mon_Vector* vector) {
    MON_CANT_BE_NULL(vector);

    return vector->_count;
}

Mon_RetCode Mon_VectorPush(Mon_Vector* vector, const void* obj) {
    MON_CANT_BE_NULL(vector);

    if (vector->_count == vector->_cap) {
        int newCap = vector->_cap * 2;

        const void** newMem;        
        if (vector->_arr != NULL) {
            newMem = Mon_Realloc(vector->_arr, newCap * sizeof(void*));
        } else {
            newMem = Mon_Alloc(newCap * sizeof(void*));
        }

        if (newMem == NULL) {
            return MON_ERR_NOMEM;
        }

        vector->_arr = newMem;
        vector->_cap = newCap;
    }

    vector->_arr[vector->_count++] = obj;

    return MON_SUCCESS;
}

void Mon_VectorRemove(Mon_Vector* vector, int index) {
    MON_CANT_BE_NULL(vector);
    MON_ASSERT(((unsigned int)index) < (unsigned int)vector->_count, 
        "Specified index (%d) cannot be >= vector element count (%d).",
        index, vector->_count);

    vector->_count--;
    for (int i = index; i < vector->_count; ++i) {
        vector->_arr[i] = vector->_arr[i + 1];
    }
    
    if (vector->_cap > MIN_SIZE && vector->_count < vector->_cap/3) {
        // In pop operation, it is not strictly necessary to resize the array.
        // So, if reallocation fails, we just keep the original one.
        int halfCap = vector->_cap/2;
        int newCap = halfCap > MIN_SIZE ? halfCap : MIN_SIZE;
        
        const void** newMem;
        if (vector->_arr != NULL) {
            newMem = Mon_Realloc(vector->_arr, newCap * sizeof(void*));
        } else {
            newMem = Mon_Alloc(newCap * sizeof(void*));
        }

        if (newMem != NULL) {
            vector->_arr = newMem;
            vector->_cap = newCap;
        }
    }
}

void Mon_VectorRemoveLast(Mon_Vector* vector) {
    Mon_VectorRemove(vector, vector->_count-1);
}

bool Mon_VectorEmpty(const Mon_Vector* vector) {
    return vector == NULL || vector->_count == 0;
}

void Mon_VectorClear(Mon_Vector* vector) {
    MON_CANT_BE_NULL(vector);
    
    vector->_count = 0;
}

void Mon_VectorClaim(Mon_Vector* vector, void** outPtr, int* count, int* capacity) {
    MON_CANT_BE_NULL(vector);
    MON_CANT_BE_NULL(outPtr);

    if (count != NULL) {
        *count = vector->_count;
    }
    if (capacity != NULL) {
        *capacity = vector->_cap;
    }

    *outPtr = vector->_arr;

    vector->_arr = NULL;
    vector->_cap = 0;
    vector->_count = 0;
}

int Mon_VectorGetIndex(const Mon_Vector* vector, const void* elem) {
    MON_CANT_BE_NULL(vector);
    
    for (int i = 0; i < vector->_count; ++i) {
        if (vector->_arr[i] == elem) {
            return i;
        }
    }

    return -1;
}

bool Mon_VectorContains(const Mon_Vector* vector, const void* elem) {
    MON_CANT_BE_NULL(vector);
    return Mon_VectorGetIndex(vector, elem) != -1;
}

void Mon_VectorFinalize(Mon_Vector* vector) {
    if (vector == NULL) {
        return;
    }

    if (vector->_arr != NULL) {
        Mon_Free(vector->_arr);
    }
    
    vector->_cap = 0;
    vector->_count = 0;
    vector->_arr = NULL;
}