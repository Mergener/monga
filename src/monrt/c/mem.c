#include "mem.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "err.h"
#include "montypes.h"

#include "gc.h"

void* RtInternal_GcAllocArray(Mon_TSize elemSize, Mon_TSize elemCount) {
    RtArray* ret = GC_malloc(elemSize * elemCount + sizeof(RtArray));

    ret->elementCount = elemCount;

    return ret;
}

void* RtInternal_GcAlloc(Mon_Intptr size) {
    void* ret = GC_malloc(size);
    if (ret == NULL) {
        FatalError(RT_ERR_NOMEM);
    }
    return ret;
}

void* RawAlloc(Mon_TSize size) {
    void* mem = malloc(size);
    if (mem == NULL) {
        FatalError(RT_ERR_NOMEM);
    }
    return mem;
}

void RawFree(void* ptr) {
    free(ptr);
}

Mon_TSize RtInternal_GetGcArrayElemCount(const RtArray* arr) {
    if (arr == NULL) {
        return 0;
    }
    return arr->elementCount;
}