#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

#include "montypes.h"

void GcCollect();
void* RtInternal_GcAlloc(Mon_Intptr size);
Mon_Int RtInternal_GetAllocSize(void* ptr);
void* RawAlloc(Mon_Intptr size);
void RawFree(void* ptr);

#endif // MEM_H