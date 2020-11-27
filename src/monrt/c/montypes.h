#ifndef MONTYPES_H
#define MONTYPES_H

#include <stdint.h>

typedef struct Mon_Str_ Mon_Str;
typedef intptr_t    Mon_Intptr;
typedef int8_t      Mon_Byte;
typedef char        Mon_Char;
typedef int16_t     Mon_Short;
typedef int32_t     Mon_Int;
typedef int64_t     Mon_Long;
typedef float       Mon_Float;
typedef double      Mon_Double;

Mon_Int ParseInt(const Mon_Str* str);
Mon_Long ParseLong(const Mon_Str* str);

#endif // MONTYPES_H