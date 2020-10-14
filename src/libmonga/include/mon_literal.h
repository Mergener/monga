#ifndef MON_LITERAL_H
#define MON_LITERAL_H

#include <stdlib.h>

typedef enum {

    MON_LIT_STR,
    MON_LIT_FLOAT,
    MON_LIT_INT

} Mon_LiteralKind;

typedef struct {

    Mon_LiteralKind literalKind;

    union {
        struct {
            char*  arr;
            size_t len;
        } string;

        double real;
        long integer;
    };

} Mon_Literal;

#endif // MON_LITERAL_H