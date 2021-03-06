#include "mon_alloc.h"

#include <stdbool.h>
#include <stdio.h>

#include "mon_error.h"

static bool s_AllocatorSet = false;

static Mon_Allocator s_Allocator;

static void UseDefaultAllocator() {
    Mon_Allocator allocator;

    allocator.alloc = malloc;
    allocator.allocZero = calloc;
    allocator.free = free;
    allocator.realloc = realloc;

    Mon_SetAllocator(allocator);
}

bool Mon_SetAllocator(Mon_Allocator allocator) {
    if (allocator.alloc == NULL ||
        allocator.allocZero == NULL ||
        allocator.free == NULL ||
        allocator.realloc == NULL) {
        return false;
    }

    s_AllocatorSet = true;

    s_Allocator = allocator;

    return true;
}

void* Mon_Alloc(size_t s) {
    if (!s_AllocatorSet) {
        UseDefaultAllocator();
    }

    void* mem = s_Allocator.alloc(s);
    return mem;
}

void* Mon_AllocZero(size_t n, size_t s) {
    if (!s_AllocatorSet) {
        UseDefaultAllocator();
    }

    return s_Allocator.allocZero(n, s);
}

void* Mon_Realloc(void* oldMem, size_t s) {
    if (!s_AllocatorSet) {
        UseDefaultAllocator();
    }

    return s_Allocator.realloc(oldMem, s);
}

void Mon_Free(void* mem) {
    if (!s_AllocatorSet) {
        UseDefaultAllocator();
    }

    s_Allocator.free(mem);
}
