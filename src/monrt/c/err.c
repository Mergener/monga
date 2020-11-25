#include "err.h"

#include <stdlib.h>

void FatalError(RtError errCode) {
    exit(errCode);
}