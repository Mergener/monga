#include "strutils.h"

#include <string.h>
#include <mon_debug.h>

#include "mon_alloc.h"

char* DuplicateString(const char* s, size_t len) {
    MON_CANT_BE_NULL(s);

    char* ret = Mon_Alloc(sizeof(char) * len + sizeof(char));
    if (ret == NULL) {
        return NULL;
    }

    memcpy(ret, s, len);

    ret[len] = '\0';

    return ret;
}