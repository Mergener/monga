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

void ProcessStringLiteral(const char* s, char** outBuf, int* outLen) {
    MON_CANT_BE_NULL(s);
    MON_CANT_BE_NULL(outBuf);
    MON_CANT_BE_NULL(outLen);

    int len = strlen(s);
    char* newStr = Mon_Alloc(len + 1);
    if (newStr == NULL) {
        *outBuf = NULL;
        *outLen = -1;
        return;
    }

    // Ignore double quotes
    int i = 0;
    int j = 0;
    if (s[0] == '\"') {
        len -= 2;
        i++;
    }

    for (; s[i] != '\0'; ++i) {
        if (s[i] != '\\') {
            newStr[j] = s[i];
        } else {
            len--;
            ++i;
            switch (s[i]) {
                case '0': 
                    newStr[j] = '\0';
                    break;

                case 'A':
                case 'a': 
                    newStr[j] = '\a';
                    break;

                case 'B':
                case 'b': 
                    newStr[j] = '\b';
                    break;

                case 'T':
                case 't': 
                    newStr[j] = '\t';
                    break;

                case 'N':
                case 'n': 
                    newStr[j] = '\n';
                    break;

                case 'V':
                case 'v': 
                    newStr[j] = '\v';
                    break;

                case 'F':
                case 'f': 
                    newStr[j] = '\f';
                    break;

                case 'R':
                case 'r': 
                    newStr[j] = '\r';
                    break;

                default:
                    newStr[j] = s[i];
                    break;
            }
        }
        ++j;
    }
    *outBuf = newStr;
    (*outBuf)[len] = '\0';
    *outLen = len;
}