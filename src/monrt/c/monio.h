#ifndef MONIO_H
#define MONIO_H

#include "montypes.h"
#include "monstr.h"
#include "mem.h"

#include <stdio.h>

Mon_Str* Input();

typedef struct {
    FILE* handle;
    Mon_Int isClosed;
} FileStream;

typedef Mon_Long FilePos;

FileStream* OpenFile(const Mon_Str* path, const Mon_Str* mode);
RtArray* ReadFileBytes(FileStream* stream, Mon_Int nbytes);
Mon_Str* ReadFileString(FileStream* stream, Mon_Int nbytes);
Mon_Str* ReadFileLine(FileStream* stream);
FilePos FileStreamTell(const FileStream* stream);
void FileStreamSeek(FileStream* stream, FilePos pos);
void CloseFile(FileStream* stream);

void RtInternal_EchoInteger(Mon_Long integer);
void RtInternal_EchoReal(Mon_Double real);
void RtInternal_EchoChar(Mon_Char c);
void RtInternal_EchoArray(void* ptr);
void RtInternal_EchoObject(void* ptr);
void RtInternal_EchoString(Mon_Str* str);

#endif // MONIO_H