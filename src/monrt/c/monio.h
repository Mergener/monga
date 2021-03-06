#ifndef MONIO_H
#define MONIO_H

#include "montypes.h"
#include "monstr.h"

void PrintInteger(int i);
void PrintFloat(float f);
void PrintString(const Mon_Str* str);

Mon_Str* Input();

void RtInternal_EchoInteger(Mon_Long integer);
void RtInternal_EchoReal(Mon_Double real);
void RtInternal_EchoChar(Mon_Char c);
void RtInternal_EchoArray(void* ptr);
void RtInternal_EchoObject(void* ptr);
void RtInternal_EchoString(Mon_Str* str);

#endif // MONIO_H