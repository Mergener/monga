#ifndef ALLOC_H
#define ALLOC_H

#include "defines.h"

#include <stdlib.h>

C_LINKAGE_BEGIN

typedef void* (MON_CALL *Mon_AllocFuncT)(size_t);
typedef void* (MON_CALL *Mon_AllocZeroFuncT)(size_t count, size_t size);
typedef void* (MON_CALL *Mon_ReallocFuncT)(void*, size_t);
typedef void  (MON_CALL *Mon_FreeFuncT)(void*);

MON_API void* MON_CALL Mon_Alloc(size_t s);
MON_API void* MON_CALL Mon_Realloc(void* oldMem, size_t s);
MON_API void  MON_CALL Mon_Free(void* mem);

C_LINKAGE_END

#endif // ALLOC_H