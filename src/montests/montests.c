#include <stdio.h>
#include <stdbool.h>

#include "mon_debug.h"

// Test files:

#include "vectortests.c"

///

typedef struct {

    const char* testName;
    void (*testFunc)();

} Test;

/** Total amount of tests performed by RunTest. */
static int s_TestCount = 0;

/** Total amount of tests ran by RunTest that passed. */
static int s_PassedCount = 0;

/** True if an assertion failed in the last test. */
static bool s_LastFailed = false;

static struct {

    const char* fileName;
    const char* funcName;
    int line;
    bool hasMsg;
    char msgBuf[512];

} s_LastAssertionError;

/** 
 * 	Runs a test, incrementing test counters. 
 * 	Before the test begins, sets s_LastFailed to false.
 */
static void RunTest(const Test* test) {
    s_TestCount++;

    test->testFunc();
    if (s_LastFailed) {
        printf("Test %s: %s\n", test->testName, "FAILED");
    } else {
        printf("Test %s: %s\n", test->testName, "PASSED");
    }
}

/** Assertion error procedure. Will set s_LastFailed to true if called. */
static void AssertError(const char* fileName,
                        const char* funcName,
                        int line,
                        const char* msgFmt,
                        va_list msgArgs) {
    s_LastFailed = true;
    s_LastAssertionError.fileName = fileName;
    s_LastAssertionError.funcName = funcName;
    s_LastAssertionError.line = line;

    if (msgFmt != NULL) {
        vsnprintf(s_LastAssertionError.msgBuf, sizeof(s_LastAssertionError.msgBuf), msgFmt, msgArgs);
        s_LastAssertionError.hasMsg = true;
    } else {
        s_LastAssertionError.hasMsg = false;
    }
}

int main() {

    printf("Starting Monga unit tests.\n");

    Mon_SetAssertErrProc(AssertError);

    printf("Tests finished. (%d of %d passed)\n", s_PassedCount, s_TestCount);

    return s_TestCount - s_PassedCount;

}