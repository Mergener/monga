#include "mem.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "err.h"
#include "montypes.h"

#include "gc.h"

#define SMALL_BUFFER_SIZE 32
#define SMALL_BUFFER_COUNT 16

typedef struct {
    bool occupied;
    Mon_Byte bytes[SMALL_BUFFER_SIZE];
} SmallBuffer;

static SmallBuffer s_SmallBuffers[SMALL_BUFFER_COUNT];

typedef struct {

    Mon_Intptr size;

} AllocHeader;

static void* Alloc(Mon_Intptr size) {
    AllocHeader* header = calloc(1, size + sizeof(AllocHeader));
    if (!header) {
        exit(EXIT_FAILURE);
    }
    header->size = size;
    return (void*)(header + 1);
}

void* RtInternal_GcAlloc(Mon_Intptr size) {
    return GC_malloc(size);
}

void* RawAlloc(Mon_Intptr size) {
    if (size <= SMALL_BUFFER_SIZE) {
        for (Mon_Int i = 0; i < SMALL_BUFFER_COUNT; ++i) {
            if (!s_SmallBuffers[i].occupied) {
                s_SmallBuffers[i].occupied = true;
                memset(s_SmallBuffers[i].bytes, 0, SMALL_BUFFER_SIZE);
                return s_SmallBuffers[i].bytes;
            }
        }
    }

    return Alloc(size);
}

void RawFree(void* ptr) {
    if (ptr >= (void*)s_SmallBuffers && ptr < (void*)&s_SmallBuffers[SMALL_BUFFER_COUNT]) {
        SmallBuffer* p = (SmallBuffer*)(ptr - offsetof(SmallBuffer, bytes));
        p->occupied = false;
        return;
    }

    free(ptr);
}

Mon_Intptr RtInternal_GetAllocSize(void* ptr);
