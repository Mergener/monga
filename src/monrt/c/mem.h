#ifndef MEM_H
#define MEM_H

#include <stdlib.h>

void GcCollect();
void* GcAlloc(size_t size);
void* RawAlloc(size_t size);
void RawFree(void* ptr);

#endif // MEM_H