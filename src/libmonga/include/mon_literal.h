#ifndef MON_LITERAL_H
#define MON_LITERAL_H

#include <mon_defines.h>

#include <stdlib.h>

C_LINKAGE_BEGIN

typedef char Mon_Char;

typedef enum {

    MON_LIT_STR,
    MON_LIT_FLOAT,
    MON_LIT_INT,
    MON_LIT_CHAR

} Mon_LiteralKind;

typedef struct {

    Mon_LiteralKind literalKind;

    union {
        struct {
            char*  arr;
            size_t len;
        } string;

        Mon_Char character;
        double real;
        long long integer;
    };

} Mon_Literal;

MON_PUBLIC Mon_Literal MON_CALL Mon_LiteralInt(long long integer);
MON_PUBLIC Mon_Literal MON_CALL Mon_LiteralReal(double real);
MON_PUBLIC Mon_Literal MON_CALL Mon_LiteralChar(Mon_Char character);
MON_PUBLIC Mon_Literal MON_CALL Mon_LiteralString(char* arr, size_t len);

C_LINKAGE_END

#endif // MON_LITERAL_H