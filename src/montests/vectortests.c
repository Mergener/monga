#include "mon_vector.h"

#include <stdint.h>

static void AddAndGet1() {
    Mon_Vector vector;
    Mon_VectorInit(&vector);

    void* ptr = (void*)5000;

    Mon_VectorPush(&vector, ptr);
    int count = Mon_VectorGetCount(&vector);
    MON_ASSERT(count == 1, "vector count must be 1. (was %d)", count);

    void* ret = Mon_VectorGet(&vector, 0);
    MON_ASSERT(ret == (void*)5000, "ptr must be == ret.");

    Mon_VectorFinalize(&vector);
}

static void InitAndFinalize() {
    int initialAllocCount = GetAllocCount();

    Mon_Vector vector;
    Mon_VectorInit(&vector);

    MON_ASSERT(Mon_VectorGetCount(&vector) == 0, "count must be zero.");
    MON_ASSERT(Mon_VectorEmpty(&vector), "vector must be emtpy.");

    Mon_VectorFinalize(&vector);
    MON_ASSERT(GetAllocCount() == initialAllocCount, "vector hasn't freed allocated memory. (initialAlloc: %d, currAlloc: %d)", 
        initialAllocCount,
        GetAllocCount());
}

static void AddLotsOfElements() {
    int initialAllocCount = GetAllocCount();

    Mon_Vector vector;
    Mon_VectorInit(&vector);

    for (int i = 0; i < 50000; ++i) {
        Mon_VectorPush(&vector, (void*)(uintptr_t)i);
    }

    Mon_VectorFinalize(&vector);
    MON_ASSERT(GetAllocCount() == initialAllocCount, "vector hasn't freed allocated memory. (initialAlloc: %d, currAlloc: %d)", 
        initialAllocCount,
        GetAllocCount());
}

static Test s_VectorTests[] = { 
    { "Add and get 1", AddAndGet1 },
    { "Init and finalize", InitAndFinalize },
    { "Add lots of elements", AddLotsOfElements }
};

void RunVectorTests() {
    printf("-> Starting Mon_Vector tests.\n");
    RunTests(s_VectorTests, sizeof(s_VectorTests) / sizeof(*s_VectorTests));
}