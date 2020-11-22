#ifndef STRLIB_H
#define STRLIB_H

typedef char Mon_Char;

typedef struct {
    int length;
    int hash;
    Mon_Char buf[];
} Mon_Str;

Mon_Str* NewString(const Mon_Char* cstr, int len);


//
//  Monga standard library functions:
//

Mon_Str* Substr(Mon_Str* str, int begin, int len);
Mon_Str* Strconcat(Mon_Str* stra, Mon_Str* strb);
int Strfind(Mon_Str* s, Mon_Str* content);
Mon_Str* Strrepl(Mon_Str* s);

#endif // STRLIB_H