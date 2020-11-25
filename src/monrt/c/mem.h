#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

void GcCollect();
void* RtInternal_GcAlloc(int size);
void* RawAlloc(size_t size);
void RawFree(void* ptr);

#endif // MEM_H