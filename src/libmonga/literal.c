#include "mon_literal.h"

#include "strutils.h"

Mon_Literal Mon_LiteralInt(long long integer) {
    Mon_Literal ret;

    ret.literalKind = MON_LIT_INT;
    ret.integer = integer;

    return ret;
}

Mon_Literal Mon_LiteralReal(double real) {
    Mon_Literal ret;

    ret.literalKind = MON_LIT_FLOAT;
    ret.real = real;

    return ret;
}

Mon_Literal Mon_LiteralChar(Mon_Char character) {
    Mon_Literal ret;

    ret.literalKind = MON_LIT_CHAR;
    ret.character = character;

    return ret;
}

Mon_Literal Mon_LiteralString(char* arr, size_t len) {
    Mon_Literal ret;

    ret.literalKind = MON_LIT_STR;
    ret.string.arr = arr;
    ret.string.len = len;

    return ret;
}
