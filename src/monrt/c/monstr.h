#ifndef STRLIB_H
#define STRLIB_H

#include "montypes.h"

struct Mon_Str_ {
    Mon_Int length;
    Mon_Int hash;
    Mon_Char buf[];
};

Mon_Str* RtInternal_StrFromSZ(const Mon_Char* sz);
Mon_Char* RtInternal_CharArrayFromStr(Mon_Str* str);

//
//  Monga standard library functions:
//

Mon_Str* Substr(Mon_Str* str, Mon_Int begin, Mon_Int len);
Mon_Str* Strconcat(Mon_Str* stra, Mon_Str* strb);
int Strfind(Mon_Str* s, Mon_Str* content);
Mon_Str* Strrepl(Mon_Str* s);

#endif // STRLIB_H