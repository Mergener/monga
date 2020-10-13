#include "montests.h"
#include "mon_vector.h"

#include <stdint.h>

static void AddAndGet1() {
    Mon_Vector vector;
    Mon_VectorInit(&vector);

    void* ptr = (void*)5000;

    Mon_VectorPush(&vector, ptr);
    int count = Mon_VectorCount(&vector);
    MON_ASSERT(count == 1, "vector count must be 1. (was %d)", count);

    void* ret = Mon_VectorGet(&vector, 0);
    MON_ASSERT(ret == (void*)5000, "ptr must be == ret.");

    Mon_VectorFinalize(&vector);
}

static void InitAndFinalize() {
    int initialAllocCount = GetAllocCount();

    Mon_Vector vector;
    Mon_VectorInit(&vector);

    MON_ASSERT(Mon_VectorCount(&vector) == 0, "count must be zero.");
    MON_ASSERT(Mon_VectorEmpty(&vector), "vector must be emtpy.");

    Mon_VectorFinalize(&vector);
    MON_ASSERT(GetAllocCount() == initialAllocCount, "vector hasn't freed allocated memory. (initialAlloc: %d, currAlloc: %d)", 
        initialAllocCount,
        GetAllocCount());
}

static void Add50kValues() {
    int initialAllocCount = GetAllocCount();

    Mon_Vector vector;
    Mon_VectorInit(&vector);

    for (int i = 0; i < 50000; ++i) {
        Mon_VectorPush(&vector, (void*)(uintptr_t)i);

        int count = Mon_VectorCount(&vector);
        MON_ASSERT(count == i + 1, "vector element count wasn't correct. (expected %d, got %d)", i + 1, count);
    }

    Mon_VectorFinalize(&vector);
    MON_ASSERT(GetAllocCount() == initialAllocCount, "vector hasn't freed allocated memory. (initialAlloc: %d, currAlloc: %d)", 
        initialAllocCount,
        GetAllocCount());
}

static void VectorClear() {
    int initialAllocCount = GetAllocCount();

    Mon_Vector vector;
    Mon_VectorInit(&vector);

    for (int i = 0; i < 500; ++i) {
        Mon_RetCode ret = Mon_VectorPush(&vector, (void*)(uintptr_t)i);
        MON_ASSERT(ret == MON_SUCCESS || ret == MON_ERR_NOMEM, 
            "Mon_VectorPush should yield MON_SUCCESS or MON_ERR_NOMEM (got %d).", 
            (int)ret);

        int count = Mon_VectorCount(&vector);
        MON_ASSERT(count == i + 1, "vector element count wasn't correct. (expected %d, got %d)", i + 1, count);
    }

    Mon_VectorClear(&vector);
    MON_ASSERT(Mon_VectorEmpty(&vector), "vector must be empty after call to Clear.");

    Mon_VectorFinalize(&vector);
    MON_ASSERT(GetAllocCount() == initialAllocCount, "vector hasn't freed allocated memory. (initialAlloc: %d, currAlloc: %d)", 
        initialAllocCount,
        GetAllocCount());
}

static void AddRemovePowerOf2() {
    int initialAllocCount = GetAllocCount();

    Mon_Vector vector;
    Mon_VectorInit(&vector);

    for (int i = 0; i < 31; ++i) {
        Mon_VectorPush(&vector, (void*)(uintptr_t)i);

        int count = Mon_VectorCount(&vector);
        MON_ASSERT(count == i + 1, "vector element count wasn't correct. (expected %d, got %d)", i + 1, count);
    }

    Mon_VectorFinalize(&vector);
    MON_ASSERT(GetAllocCount() == initialAllocCount, "vector hasn't freed allocated memory. (initialAlloc: %d, currAlloc: %d)", 
        initialAllocCount,
        GetAllocCount());
}

static void NullVectorEmpty() {
    bool ret = Mon_VectorEmpty(NULL);
    MON_ASSERT(ret, "NULL vector must be empty.");
}

static Test s_VectorTests[] = { 
    { "Add and get 1", AddAndGet1 },
    { "Init and finalize", InitAndFinalize },
    { "Add 50000 values", Add50kValues },
    { "Add remove close to power of 2", AddRemovePowerOf2 },
    { "Null vector must be empty", NullVectorEmpty },
    { "Clear", VectorClear }
};

void RunVectorTests() {
    printf("-> Starting Mon_Vector tests.\n");
    RunTests(s_VectorTests, sizeof(s_VectorTests) / sizeof(*s_VectorTests));
}