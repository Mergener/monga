#include "monio.h"

#include <stdio.h>

int readInt() {
    int ret;
    scanf("%d", &ret);
    return ret;
}

void printInteger(int i) {
    printf("%d\n", i);
}

void printFloat(float f) {
    printf("%f\n", f);
}

void printString(const Mon_Str* str) {
    fwrite(str->buf, sizeof(char), str->length, stdout);
    putc('\n', stdout);
}
