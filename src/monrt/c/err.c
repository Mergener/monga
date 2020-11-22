#include "err.h"

#include <stdio.h>

void FatalError(FatalErrorCode err,
                const char* file,
                int line,
                const char* function,
                const char* overrideMsg) {
    fprintf(stderr, "******\nFATAL ERROR! (at %s, %s, line %d)\n", file, function, line);

    if (overrideMsg != NULL) {
        fprintf(stderr, "%s\n", overrideMsg);
    } else {
        switch (err) {
            case FATAL_NOMEM:
                fprintf(stderr, "The program ran out of memory.\n");
                break;

            case FATAL_BADARG:
                fprintf(stderr, "An invalid argument was passed to a function.\n");
                break;

            default:
                fprintf(stderr, "An unknown error has occurred.\n");
                exit(FATAL_UNKNOWN);
                break;
        }
    }

    exit(err);
}