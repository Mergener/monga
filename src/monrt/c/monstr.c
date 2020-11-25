#include "monstr.h"

#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "mem.h"

static int Hash(const Mon_Char* key, int32_t len) {
    // Uses djb2 hash algorithm.
    uint32_t hash = 5381;
    int32_t c;
    const Mon_Char* end = key + len;

    while (key != end) {
        hash = ((hash << 5) + hash) + c;
        c = *key;
        key++;
    }

    return (int32_t)hash;
}

static Mon_Str* NewString(const Mon_Char* cstr, int len) {
    if (cstr == NULL) {
        FatalError(RT_ERR_BADARG);
    } 

    Mon_Str* ret = RtInternal_GcAlloc(sizeof(Mon_Char) * len + sizeof(Mon_Char) + sizeof(Mon_Str));

    ret->hash = Hash(ret->buf, len);
    ret->length = len;
    memcpy(ret->buf, cstr, sizeof(Mon_Char) * len);
    ret->buf[ret->length] = '\0';

    return ret;
}

Mon_Str* RtInternal_StrFromSZ(const Mon_Char* sz) {
    if (sz == NULL) {
        FatalError(RT_ERR_NULLARG);
    }

    int len = strlen(sz);
    int bufSize = len * sizeof(Mon_Char) + sizeof(Mon_Char) + // buffer
                  sizeof(Mon_Str); // header
    
    Mon_Str* ret = RtInternal_GcAlloc(bufSize);

    memcpy(ret->buf, sz, len * sizeof(Mon_Char));
    ret->buf[len] = '\0';
    ret->length = len;
    ret->hash = Hash(ret->buf, len);

    return ret;
}

Mon_Char* RtInternal_CharArrayFromStr(Mon_Str* str) {
    if (str == NULL) {
        return NULL;
    }

    Mon_Char* buffer = RtInternal_GcAlloc(str->length * sizeof(Mon_Char) + sizeof(Mon_Char));
    memcpy(buffer, str->buf, str->length * sizeof(Mon_Char));
    buffer[str->length] = '\0';

    return buffer;
}

Mon_Str* Substr(Mon_Str* str, int32_t begin, int32_t len) {
    if (str == NULL) {
        FatalError(RT_ERR_NULLARG);
    }
    if (begin < 0 || begin >= str->length) {
        FatalError(RT_ERR_BADARG);
    }
    if ((len + begin) >= str->length || len < 0) {
        FatalError(RT_ERR_BADARG);
    }
    return NewString(&str->buf[begin], len);
}

Mon_Str* Strconcat(Mon_Str* stra, Mon_Str* strb) {
    if (stra == NULL) {
        FatalError(RT_ERR_NULLARG);
    }
    if (strb == NULL) {
        FatalError(RT_ERR_NULLARG);
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

int Strfind(Mon_Str* s, Mon_Str* content) {
    if (s == NULL) {
        FatalError(RT_ERR_NULLARG);
    }
    if (content == NULL) {
        FatalError(RT_ERR_NULLARG);
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

Mon_Str* Strrepl(Mon_Str* s) {
    return NULL;
}
