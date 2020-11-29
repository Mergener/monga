#include "mem.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "err.h"
#include "montypes.h"

#include "gc.h"


void* RtInternal_GcAlloc(Mon_Intptr size) {
    return GC_malloc(size);
}

void* RawAlloc(Mon_Intptr size) {
    void* mem = malloc(size);
    if (mem == NULL) {
        FatalError(RT_ERR_NOMEM);
    }
    return mem;
}

void RawFree(void* ptr) {
    free(ptr);
}

Mon_Intptr RtInternal_GetAllocSize(void* ptr);
