#include "mon_debug.h"

#include <stdio.h>

static Mon_AssertErrProc s_AssertProc = NULL;

static void DefaultAssertProc(const char* fileName,
                              const char* funcName,
                              int line,
                              const char* msgFmt,
                              va_list msgArgs) {
    fprintf(stderr, "\n***\nMonga Assertion failed! (in file %s, function %s, line %d)\n\tMessage: ", fileName, funcName, line);
    vfprintf(stderr, msgFmt, msgArgs);
    fprintf(stderr, "\n***\n");
    exit(EXIT_FAILURE);
}

void Mon_AssertInternal(bool cond,
                        const char* fileName,
                        const char* funcName,
                        int line,
                        const char* msgFmt,
                        ...) {

    if (cond) {
        // Condition was met, no errors should be thrown
        return;
    }

    va_list args;
    va_start(args, msgFmt);

    if (s_AssertProc == NULL) {
        DefaultAssertProc(fileName, funcName, line, msgFmt, args);
    } else {
        s_AssertProc(fileName, funcName, line, msgFmt, args);
    }

    va_end(args);
}

void Mon_SetAssertErrProc(Mon_AssertErrProc assertProc) {
    if (assertProc == NULL) {
        return;
    }

    s_AssertProc = assertProc;
}