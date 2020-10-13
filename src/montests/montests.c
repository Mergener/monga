#include "montests.h"

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "mon_debug.h"
#include "mon_alloc.h"

// Test files:

#include "vectortests.c"

///

/** Total amount of tests performed by RunTest. */
static int s_TestCount = 0;

/** Total amount of tests ran by RunTest that passed. */
static int s_PassedCount = 0;

/** True if an assertion failed in the last test. */
static bool s_LastFailed = false;

void RunTest(const Test* test) {
    s_TestCount++;

    s_LastFailed = false;

    test->testFunc();
    if (s_LastFailed) {
        printf("Test %s: %s\n", test->testName, "FAILED (view assertion data above)");
    } else {
        printf("Test %s: %s\n", test->testName, "PASSED");
        s_PassedCount++;
    }
}

void RunTests(const Test* testList, int count) {
    for (int i = 0; i < count; ++i) {
        RunTest(&testList[i]);
    }
}

/** Assertion error procedure. Will set s_LastFailed to true if called. */
static void AssertError(const char* fileName,
                        const char* funcName,
                        int line,
                        const char* msgFmt,
                        va_list msgArgs) {
    s_LastFailed = true;

    printf("\n*** ASSERTION FAILED ***\n*\n");
    printf("* File: %s\n", fileName);
    printf("* Line: %d\n", line);
    printf("* Function: %s\n", funcName);

    if (msgFmt != NULL) {
        printf("* Message: \"");
        vprintf(msgFmt, msgArgs);
        printf("\"\n");
    }

    printf("*\n************\n\n");
}

typedef struct AllocNode_ {

    struct AllocNode_* next;
    size_t size;

} AllocNode;
static AllocNode* s_AllocHead = NULL;
static int s_AllocCount = 0;

#define MEM_TO_NODE(mem)  ((AllocNode*) ( ((const uint8_t*)(mem))  - sizeof(AllocNode) ))
#define NODE_TO_MEM(node) ((void*)      ( ((const uint8_t*)(node)) + sizeof(AllocNode) ))

/**
 *  Returns the 'next' field of a node in which its
 *  subsequent node refers to the specified pointer,
 *  's_AllocHead' if the alloc node chain head refers to 'mem' or
 *  NULL if mem is not an allocated space.
 */
static AllocNode** PrevNodeNext(void* mem) {
    if (s_AllocHead == NULL) {
        return NULL;
    }

    if (MEM_TO_NODE(mem) == s_AllocHead) {
        return &s_AllocHead;
    }

    AllocNode* it = s_AllocHead;
    AllocNode* memNode = MEM_TO_NODE(mem);
    while (it->next != NULL) {
        if (it->next == memNode) {
            return &it->next;
        }
    }

    return NULL;
}

static AllocNode* FindAllocNode(void* mem) {
    AllocNode** prevNext = PrevNodeNext(mem);
    if (prevNext == NULL) {
        return NULL;
    }
    return *prevNext;
}

int GetAllocCount() {
    return s_AllocCount;
}

bool IsAllocated(void* mem) {
    const AllocNode* it = s_AllocHead;

    while (it != NULL) {
        if (NODE_TO_MEM(it) == mem) {
            return true;
        }

        it = it->next;
    }
    return false;
}

static void* Alloc(size_t s) {
    AllocNode* node = malloc(s + sizeof(AllocNode));

    if (node == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(-1);
    }

    node->next = s_AllocHead;
    node->size = s;
    s_AllocHead = node;
    s_AllocCount++;

    void* ret = (void*)&node[1];

    return ret;
}

static void* AllocZero(size_t s1, size_t s2) {
    void* ret = Alloc(s1 * s2);
    if (ret == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(-1);
    }

    memset(ret, 0, s1 * s2);

    return ret;
}

static void FreeAlloc(void* mem) {
    if (mem == NULL) {
        MON_ASSERT(false, "Cannot free NULL pointer.");
        return;
    }

    AllocNode** prevNext = PrevNodeNext(mem);
    if (prevNext == NULL) {
        MON_ASSERT(false, "Cannot free non allocated pointer. (tried to free %p)", mem);
        return;
    }
    AllocNode* memNode = MEM_TO_NODE(mem);
    *prevNext = memNode->next;

    free(memNode);
    s_AllocCount--;
}

static void* ReAlloc(void* src, size_t newSize) {
    void* mem = Alloc(newSize);
    if (mem == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(-1);
    }

    AllocNode* srcNode = MEM_TO_NODE(src);
    memcpy(mem, src, srcNode->size < newSize ? srcNode->size : newSize);
    FreeAlloc(src);

    return mem;
}

#undef MEM_TO_NODE
#undef NODE_TO_MEM

static void Setup() {
    // Set custom assertion handler
    Mon_SetAssertErrProc(AssertError);

    // Set custom allocators
    Mon_Allocator allocator;
    allocator.alloc = Alloc;
    allocator.allocZero = AllocZero;
    allocator.free = FreeAlloc;
    allocator.realloc = ReAlloc;

    Mon_SetAllocator(allocator);
}

int main() {
    printf("*** Starting Libmonga unit tests. ***\n\n");

    Setup();

    // Tests:
    RunVectorTests();
    ///

    printf("\n*** Libmonga unit tests finished. (%d of %d passed) ***\n", s_PassedCount, s_TestCount);

    // Returns the number of failed tests.
    return s_TestCount - s_PassedCount;
}