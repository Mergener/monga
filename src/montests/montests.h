#ifndef MONTESTS_H
#define MONTESTS_H

#include <stdbool.h>
#include <stdio.h>

#include "mon_debug.h"

#define ASSERT_EQUAL_FILES(test, expected, testFileName, expectedFileName) \
    AssertEqualFilesInternal(test, expected, testFileName, expectedFileName, __FILE__, __func__, __LINE__)

typedef struct {

    const char* testName;
    void (*testFunc)();

} Test;

extern FILE* g_LogFileStream;

/** 
 *  Runs a test, incrementing test counters. 
 */
void RunTest(const Test* test);

/**
 *  Runs an array of tests.
 */
void RunTests(const Test* testList, int count);

/**
 *  Returns the number of 'Mon_Alloc' allocated spaces.
 */
int GetAllocCount();

/**
 *  Returns true if the specified pointer is currently allocated.
 *  Only works on pointers allocated by Mon_Alloc.
 */
bool IsAllocated(void* mem);

void* InternalAlloc(size_t b);
void* InternalRealloc(void* mem, size_t newSize);
void InternalFree(void* mem);

/**
 *  Outputs data to a log file.
 */
void Logf(const char* fmt, ...);

void AssertEqualFilesInternal(FILE* test, FILE* expected,
                              const char* testFileName,
                              const char* expectedFileName,
                              const char* srcFile, 
                              const char* funcName, 
                              int line);

#endif // MONTESTS_H