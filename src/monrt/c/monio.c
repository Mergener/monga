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
    if (str == NULL) {
        printf("(null)\n");
        return;
    }

    fwrite(str->buf, sizeof(char), str->length, stdout);
    putc('\n', stdout);
}

void RtInternal_EchoInteger(Mon_Long integer) {
    printf("%ld\n", integer);
}

void RtInternal_EchoReal(Mon_Double real) {
    printf("%f\n", real);
}

void RtInternal_EchoChar(Mon_Char c) {
    printf("%c\n", c);
}

void RtInternal_EchoArray(void* ptr) {
    if (ptr == NULL) {
        printf("(null)\n");
    } else {
        printf("[Array at %p]\n", ptr);
    }
}

void RtInternal_EchoObject(void* ptr) {
    if (ptr == NULL) {
        printf("(null)\n");
    } else {
        printf("[Object at %p]\n", ptr);
    }
}

void RtInternal_EchoString(Mon_Str* str) {
    printString(str);
}
