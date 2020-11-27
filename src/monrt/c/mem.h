#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

#include "montypes.h"

void GcCollect();
void* RawAlloc(Mon_Intptr size);
void RawFree(void* ptr);

void* RtInternal_GcAlloc(Mon_Intptr size);
Mon_Intptr RtInternal_GetAllocSize(void* ptr);


#endif // MEM_H