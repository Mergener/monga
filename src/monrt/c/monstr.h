#ifndef STRLIB_H
#define STRLIB_H

#include "montypes.h"

struct Mon_Str_ {
    Mon_TSize length;
    Mon_TSize hash;
    Mon_Char buf[];
};

static Mon_TSize RtHash(const Mon_Char* key, Mon_TSize len);
Mon_Str* RtInternal_StrFromSZ(const Mon_Char* sz);
Mon_Char* RtInternal_CharArrayFromStr(Mon_Str* str);
Mon_TSize RtInternal_Hash(const Mon_Char* key, Mon_TSize len);

//
//  Monga standard library functions:
//

Mon_Str* Substr(Mon_Str* str, Mon_TSize begin, Mon_TSize len);
Mon_Str* Strconcat(Mon_Str* stra, Mon_Str* strb);
int Strfind(Mon_Str* s, Mon_Str* content);
Mon_Str* Strrepl(Mon_Str* s);
Mon_Int StrEquals(Mon_Str* a, Mon_Str* b);

static Mon_TSize RtHash(const Mon_Char* key, Mon_TSize len) {
    // Uses djb2 hash algorithm.
    Mon_TSize hash = 5381;
    Mon_TSize c;
    const Mon_Char* end = key + len;

    c = *key;
    while (key != end) {
        hash = ((hash << 5) + hash) + c;
        c = *key;
        key++;
    }

    return hash;
}


#endif // STRLIB_H