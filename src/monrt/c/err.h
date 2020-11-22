#ifndef ERR_H
#define ERR_H

#include <stdlib.h>

#define FATAL(errType) \
    FATAL_M(errType, NULL)

#define FATAL_M(errType, msg) \
    FatalError(FATAL_##errType, __FILE__, __LINE__, __func__, msg)

typedef enum {

    FATAL_UNKNOWN = -1,
    FATAL_NOMEM = -2,
    FATAL_BADARG = -3

} FatalErrorCode;

void FatalError(FatalErrorCode err,
                const char* file,
                int line,
                const char* function,
                const char* overrideMsg);

#endif // ERR_H