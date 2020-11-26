#ifndef MONIO_H
#define MONIO_H

#include "montypes.h"
#include "monstr.h"

void printInteger(int i);
void printFloat(float f);
void printString(const Mon_Str* str);
int readInt();

void RtInternal_EchoInteger(Mon_Long integer);
void RtInternal_EchoReal(Mon_Double real);
void RtInternal_EchoChar(Mon_Char c);
void RtInternal_EchoArray(void* ptr);
void RtInternal_EchoObject(void* ptr);
void RtInternal_EchoString(Mon_Str* str);

#endif // MONIO_H