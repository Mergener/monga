#ifndef MONTESTS_H
#define MONTESTS_H

#include <stdbool.h>
#include <stdio.h>

#include "mon_debug.h"

typedef struct {

    const char* testName;
    void (*testFunc)();

} Test;

extern FILE* g_LogFileStream;

/** 
 *     Runs a test, incrementing test counters. 
 */
void RunTest(const Test* test);

/**
 *    Runs an array of tests.
 */
void RunTests(const Test* testList, int count);

/**
 *  Returns the number of allocated spaces.
 */
int GetAllocCount();

/**
 *  Returns true if the specified pointer is currently allocated.
 */
bool IsAllocated(void* mem);

/**
 *  Outputs data to a log file.
 */
void Logf(const char* fmt, ...);

#endif // MONTESTS_H