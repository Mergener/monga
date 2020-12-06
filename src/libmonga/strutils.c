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

char ConvertControlCharacter(char c) {
    switch (c) {
        case '0': 
            return '\0';

        case 'A':
        case 'a': 
            return '\a';

        case 'B':
        case 'b': 
            return '\b';

        case 'T':
        case 't': 
            return '\t';

        case 'N':
        case 'n': 
            return '\n';

        case 'V':
        case 'v': 
            return '\v';

        case 'F':
        case 'f': 
            return '\f';

        case 'R':
        case 'r': 
            return '\r';

        default:
            return c;
    }
}

void ProcessStringLiteral(const char* s, size_t len, char** outBuf, size_t* outLen) {
    MON_CANT_BE_NULL(s);
    MON_CANT_BE_NULL(outBuf);
    MON_CANT_BE_NULL(outLen);

    char* newStr = Mon_Alloc(len + 1);
    if (newStr == NULL) {
        *outBuf = NULL;
        *outLen = -1;
        return;
    }

    size_t i = 0,
           j = 0;

    while (s[i] != '\0' && i < len) {
        
        if (s[i] != '\\') {
            newStr[j] = s[i];
        } else {
            --len; ++i;
            newStr[j] = ConvertControlCharacter(s[i]);
        }

        ++j; ++i;
    }

    *outBuf = newStr;
    (*outBuf)[len] = '\0';
    *outLen = len;
}