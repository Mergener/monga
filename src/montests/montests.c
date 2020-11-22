#include "montests.h"

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "mon_debug.h"
#include "mon_alloc.h"

// Test files:

#include "asttests.c"
#include "dicttests.c"
#include "vectortests.c"

///

static void OutOfMemoryError();

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
        printf("[Test] %s (%s) \n", test->testName, "FAILED - view assertion data above");
    } else {
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
        it = it->next;
    }

    return NULL;
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
    AllocNode* node = InternalAlloc(s + sizeof(AllocNode));

    node->next = s_AllocHead;
    node->size = s;
    s_AllocHead = node;
    s_AllocCount++;

    void* ret = (void*)&node[1];

    Logf("[Memory] %ld bytes allocated at %p.\n", (long)s, ret);

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

    size_t size = memNode->size;

    InternalFree(memNode);
    s_AllocCount--;

    Logf("[Memory] %ld bytes freed at %p.\n", (long)size, mem);
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

FILE* g_LogFileStream = NULL;

void Logf(const char* fmt, ...) {
    if (g_LogFileStream == NULL) {
        // Maybe it would be a better idea to print to stdout instead
        // of doing nothing, but that would pollute assertion results and
        // passed/failed test outputs.
        return;
    }

    va_list args;
    va_start(args, fmt);

    vfprintf(g_LogFileStream, fmt, args);

    va_end(args);
}

void* InternalAlloc(size_t b) {
    void* mem = malloc(b);
    if (mem == NULL) {
        OutOfMemoryError();
    }
    return mem;
}

void* InternalRealloc(void* mem, size_t newSize) {
    void* newMem = realloc(mem, newSize);
    if (newMem == NULL) {
        OutOfMemoryError();
    }
    return newMem;
}

void InternalFree(void* mem) {
    free(mem);
}

static void OutOfMemoryError() {
    fprintf(stderr, "An allocation failure occurred during tests execution.\n");
    exit(EXIT_FAILURE);
}

static void Setup(const char* logFilePath) {
    // Set custom assertion handler
    Mon_SetAssertErrProc(AssertError);

    // Set custom allocators
    Mon_Allocator allocator;
    allocator.alloc = Alloc;
    allocator.allocZero = AllocZero;
    allocator.free = FreeAlloc;
    allocator.realloc = ReAlloc;

    Mon_SetAllocator(allocator);

    // Open log file
    if (logFilePath != NULL) {
        g_LogFileStream = fopen(logFilePath, "w");
        if (g_LogFileStream == NULL) {
            fprintf(stderr, "Couldn't open log file.\n");
        }
    }
}

static bool s_HasCleanedUp = false;

static void Cleanup() {
    if (s_HasCleanedUp) {
        return;
    }

    s_HasCleanedUp = true;

    if (g_LogFileStream != NULL) {
        fclose(g_LogFileStream);
    }
}

int main(int argc, char* argv[]) {
    printf("*** Starting Libmonga unit tests. ***\n\n");

    atexit(Cleanup);

    const char* logFilePath;

    if (argc >= 2) {
        logFilePath = argv[1];
    } else {
        logFilePath = NULL;
    }

    Setup(logFilePath);

    // Tests:
    RunVectorTests();
    RunDictTests();
    RunASTTests();
    ///

    printf("\n*** Libmonga unit tests finished. (%d of %d passed) ***\n", s_PassedCount, s_TestCount);

    Cleanup();

    // Returns the number of failed tests.
    return s_TestCount - s_PassedCount;
}