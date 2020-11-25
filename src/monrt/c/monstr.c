#include "monstr.h"

#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "mem.h"

static int Hash(const Mon_Char* key, int len) {
    // Uses djb2 hash algorithm.
    unsigned int hash = 5381;
    int c;
    const Mon_Char* end = key + len;

    while (key != end) {
        hash = ((hash << 5) + hash) + c;
        c = *key;
        key++;
    }

    return (int)hash;
}

Mon_Str* NewString(const Mon_Char* cstr, int len) {
    if (cstr == NULL) {
        FATAL(BADARG);
    } 

    Mon_Str* ret = RtInternal_GcAlloc(sizeof(Mon_Char) * len + sizeof(Mon_Char) + sizeof(Mon_Str));

    ret->hash = Hash(ret->buf, len);
    ret->length = len;
    memcpy(ret->buf, cstr, sizeof(Mon_Char) * len);
    ret->buf[ret->length] = '\0';

    return ret;
}

Mon_Str* FUNC_SUBSTR(Mon_Str* str, int begin, int len) {
    if (str == NULL) {
        FATAL(BADARG);
    }
    if (begin < 0 || begin >= str->length) {
        FATAL(BADARG);
    }
    if ((len + begin) >= str->length || len < 0) {
        FATAL(BADARG);
    }
    return NewString(&str->buf[begin], len);
}

Mon_Str* FUNC_STRCONCAT(Mon_Str* stra, Mon_Str* strb) {
    if (stra == NULL) {
        FATAL(BADARG);
    }
    if (strb == NULL) {
        FATAL(BADARG);
    }
    
    Mon_Char stackBuf[128];
    Mon_Char* buf;

    size_t combinedLen = stra->length + strb->length;
    if (combinedLen < (sizeof(stackBuf)/sizeof(Mon_Char))) {
        buf = stackBuf;
    } else {
        buf = RawAlloc(combinedLen * sizeof(Mon_Char));
    }

    size_t asiz = stra->length * sizeof(Mon_Char);
    size_t bsiz = strb->length * sizeof(Mon_Char);
    memcpy(buf, stra->buf, asiz);
    memcpy(&buf[asiz], strb->buf, bsiz);
    
    buf[combinedLen] = '\0';

    return NewString(buf, combinedLen);
}

int FUNC_STRFIND(Mon_Str* s, Mon_Str* content) {
    if (s == NULL) {
        FATAL(BADARG);
    }
    if (content == NULL) {
        FATAL(BADARG);
    }
    
    int eqBegin = 0;
    int eqCount = 0;

    for (int i = 0; i < s->length; ++i) {
        if (eqCount == content->length) {
            return eqBegin;
        }

        if (s->buf[i] == content->buf[eqCount]) {
            eqCount++;
        } else {
            eqBegin = i + 1;
        }
    }

    return -1;
}

Mon_Str* FUNC_STRREPL(Mon_Str* s) {
    return NULL;
}
