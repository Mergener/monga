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

static void ForeachSum() {
    int initialAllocCount = GetAllocCount();

    Mon_Vector vector;
    Mon_VectorInit(&vector);

    for (int i = 0; i < 100; ++i) {
        Mon_RetCode ret = Mon_VectorPush(&vector, (void*)(uintptr_t)i + 1);
        MON_ASSERT(ret == MON_SUCCESS || ret == MON_ERR_NOMEM, 
            "Mon_VectorPush should yield MON_SUCCESS or MON_ERR_NOMEM (got %d).", 
            (int)ret);

        int count = Mon_VectorCount(&vector);
        MON_ASSERT(count == i + 1, "vector element count wasn't correct. (expected %d, got %d)", i + 1, count);
    }
    // Sum of values should be equal to 5050
    uintptr_t sum = 0;
    MON_VECTOR_FOREACH(&vector, uintptr_t, val, 
        sum += val;
    );
    MON_ASSERT(sum == 5050, "sum of elements must be equal to 5050. (got %ld)", (long)sum);

    Mon_VectorFinalize(&vector);
    MON_ASSERT(GetAllocCount() == initialAllocCount, "vector hasn't freed allocated memory. (initialAlloc: %d, currAlloc: %d)", 
        initialAllocCount,
        GetAllocCount());
}

static void ForeachEmpty() {
    int initialAllocCount = GetAllocCount();

    Mon_Vector vector;
    Mon_VectorInit(&vector);

    bool changed = false;
    MON_VECTOR_FOREACH(&vector, uintptr_t, val, 
        changed = true;
    );
    MON_ASSERT(!changed, "foreach on empty vector should do nothing.");

    Mon_VectorFinalize(&vector);
    MON_ASSERT(GetAllocCount() == initialAllocCount, "vector hasn't freed allocated memory. (initialAlloc: %d, currAlloc: %d)", 
        initialAllocCount,
        GetAllocCount());
}

static void RemoveMiddle() {
    int initialAllocCount = GetAllocCount();

    Mon_Vector vector;
    Mon_VectorInit(&vector);

    void* a = (void*)10;
    void* b = (void*)20;
    void* c = (void*)30;
    void* d = (void*)40;

    Mon_VectorPush(&vector, a);
    Mon_VectorPush(&vector, b);
    Mon_VectorPush(&vector, c);
    Mon_VectorPush(&vector, d);

    int count = Mon_VectorCount(&vector);
    MON_ASSERT(count != 4, "Wrong vector count. (expected %d, got %d)", 4, count);

    Mon_VectorRemove(&vector, 1);

    count = Mon_VectorCount(&vector);
    MON_ASSERT(count != 3, "Wrong vector count. (expected %d, got %d)", 3, count);

    void* got = Mon_VectorGet(&vector, 0);
    MON_ASSERT(got == a, "element at index 0 must be %p. (got %p)", a, got);
    got = Mon_VectorGet(&vector, 1);
    MON_ASSERT(got == c, "element at index 0 must be %p. (got %p)", c, got);
    got = Mon_VectorGet(&vector, 2);
    MON_ASSERT(got == d, "element at index 0 must be %p. (got %p)", d, got);

    Mon_VectorFinalize(&vector);
    MON_ASSERT(GetAllocCount() == initialAllocCount, "vector hasn't freed allocated memory. (initialAlloc: %d, currAlloc: %d)", 
        initialAllocCount,
        GetAllocCount());
}

static Test s_VectorTests[] = { 
    { "Add and get 1", AddAndGet1 },
    { "Init and finalize", InitAndFinalize },
    { "Add 50000 values", Add50kValues },
    { "Add remove close to power of 2", AddRemovePowerOf2 },
    { "Null vector must be empty", NullVectorEmpty },
    { "Foreach Sum", ForeachSum },
    { "Foreach Empty", ForeachEmpty },
    { "Remove Middle", ForeachEmpty },
    { "Clear", VectorClear }
};

void RunVectorTests() {
    printf("-> Starting Mon_Vector tests.\n");
    RunTests(s_VectorTests, sizeof(s_VectorTests) / sizeof(*s_VectorTests));
}