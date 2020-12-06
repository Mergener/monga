#include "monio.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "mem.h"
#include "monstr.h"
#include "err.h"

/*
FileStream* OpenFile(const Mon_Str* path, const Mon_Str* mode) {
    if (path == NULL || mode == NULL) {
        FatalError(RT_ERR_NULLARG);
    }

    FILE* f = fopen(path->buf, mode->buf);
    if (f == NULL) {
        return NULL;
    }

    FileStream* stream = RtInternal_GcAlloc(sizeof(FileStream));
    stream->handle = f;
    stream->isClosed = 0;

    return stream;
}

RtArray* ReadFileBytes(FileStream* stream, Mon_Int nbytes) {
    if (stream == NULL) {
        FatalError(RT_ERR_NULLARG);
    }
    if (stream->isClosed) {
        return NULL;
    }

    
}

Mon_Str* ReadFileString(FileStream* stream, Mon_Int nbytes) {

}

Mon_Str* ReadFileLine(FileStream* stream) {

}

FilePos FileStreamTell(const FileStream* stream) {

}

void FileStreamSeek(FileStream* stream, FilePos pos) {

}

void CloseFile(FileStream* stream) {

}
*/

void RtInternal_EchoInteger(Mon_Long integer) {
    printf("%"PRIi64, integer);
}

void RtInternal_EchoReal(Mon_Double real) {
    printf("%f", real);
}

void RtInternal_EchoChar(Mon_Char c) {
    printf("%c", c);
}

void RtInternal_EchoArray(void* ptr) {
    if (ptr == NULL) {
        printf("(null)");
    } else {
        printf("[Array at %p]", ptr);
    }
}

void RtInternal_EchoObject(void* ptr) {
    if (ptr == NULL) {
        printf("(null)");
    } else {
        printf("[Object at %p]", ptr);
    }
}

void RtInternal_EchoString(Mon_Str* str) {
    if (str == NULL) {
        printf("(null)");
        return;
    }

    fwrite(str->buf, sizeof(char), str->length, stdout);
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