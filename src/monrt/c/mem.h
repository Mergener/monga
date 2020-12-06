#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

#include "montypes.h"

typedef struct {
    
    Mon_Long elementCount;
    uint8_t data[];

} RtArray;

void GcCollect();
void* RawAlloc(Mon_TSize size);
void RawFree(void* ptr);

void* RtInternal_GcAlloc(Mon_Intptr size);
void* RtInternal_GcAllocArray(Mon_TSize elemSize, Mon_TSize elemCount);
Mon_TSize RtInternal_GetGcArrayElemCount(const RtArray* arr);

#endif // MEM_H