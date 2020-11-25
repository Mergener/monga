#include "mem.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "err.h"

#define SMALL_BUFFER_SIZE 32
#define SMALL_BUFFER_COUNT 16

typedef struct {
    bool occupied;
    char bytes[SMALL_BUFFER_SIZE];
} SmallBuffer;

static SmallBuffer s_SmallBuffers[SMALL_BUFFER_COUNT];

void* RtInternal_GcAlloc(int size) {
    return RawAlloc(size);
}

void* RawAlloc(size_t size) {
    if (size <= SMALL_BUFFER_SIZE) {
        for (int i = 0; i < SMALL_BUFFER_COUNT; ++i) {
            if (!s_SmallBuffers[i].occupied) {
                s_SmallBuffers[i].occupied = true;
                memset(s_SmallBuffers[i].bytes, 0, SMALL_BUFFER_SIZE);
                return s_SmallBuffers[i].bytes;
            }
        }
    }

    void* ret = calloc(1, size);
    if (!ret) {
        FATAL(NOMEM);
        exit(-1);
    }
    return ret;
}

void RawFree(void* ptr) {
    if (ptr >= (void*)s_SmallBuffers && ptr < (void*)&s_SmallBuffers[SMALL_BUFFER_COUNT]) {
        SmallBuffer* p = (SmallBuffer*)(ptr - offsetof(SmallBuffer, bytes));
        p->occupied = false;
        return;
    }

    free(ptr);
}
