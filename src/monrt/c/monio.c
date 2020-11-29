#include "monio.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "mem.h"
#include "monstr.h"

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

Mon_Str* Input() {

#define STACK_BUFFER_SIZE 128

    Mon_Char stackBuffer[STACK_BUFFER_SIZE];

    Mon_Char* buf = stackBuffer;

    Mon_Char* ret;
    size_t bufSize = STACK_BUFFER_SIZE;
    
    int i = 0;
    while (true) {
        int c = fgetc(stdin);
        if (c == EOF || c == '\n') {
            break;
        }

        buf[i] = c;
        ++i;

        if (i >= bufSize - 1) {
            // Reallocation is required
            Mon_Char* oldBuf = buf;

            size_t newBufSize = bufSize*2;
            buf = RawAlloc(newBufSize);
            memcpy(buf, oldBuf, bufSize);
            
            if (oldBuf != stackBuffer) {
                RawFree(oldBuf);
            }
            bufSize = newBufSize;
        }
    }

    if (buf != stackBuffer) {
        RawFree(buf);
    }
    buf[i] = '\0';

#undef STACK_BUFFER_SIZE

    return RtInternal_StrFromSZ(buf);
}