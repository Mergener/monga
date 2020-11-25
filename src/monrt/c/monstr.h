#ifndef STRLIB_H
#define STRLIB_H

#include <stdint.h>

typedef char Mon_Char;

typedef struct {
    int32_t length;
    int32_t hash;
    Mon_Char buf[];
} Mon_Str;

Mon_Str* RtInternal_StrFromSZ(const Mon_Char* sz);
Mon_Char* RtInternal_CharArrayFromStr(Mon_Str* str);

//
//  Monga standard library functions:
//

Mon_Str* Substr(Mon_Str* str, int32_t begin, int32_t len);
Mon_Str* Strconcat(Mon_Str* stra, Mon_Str* strb);
int Strfind(Mon_Str* s, Mon_Str* content);
Mon_Str* Strrepl(Mon_Str* s);

#endif // STRLIB_H