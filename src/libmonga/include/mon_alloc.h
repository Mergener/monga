#ifndef ALLOC_H
#define ALLOC_H

#include "mon_defines.h"

#include <stdlib.h>
#include <stdbool.h>

C_LINKAGE_BEGIN

typedef struct {

    /** Default allocation function. Defaults to malloc. */
    void* (*alloc)(size_t);

    /** Zero-initialized allocation function. Defaults to calloc. */
    void* (*allocZero)(size_t, size_t);

    /** Memory reallocation function. Defaults to realloc. */
    void* (*realloc)(void*, size_t);

    /** Memory disposal function. Defaults to free. */
    void  (*free)(void*);

} Mon_Allocator;

/**
 *  Provides a custom allocator to Monga. The custom allocator's functions
 *  will then be used by Monga's Mon_Alloc, Mon_Realloc and Mon_Free routines.
 * 
 *  @return True if the allocator was accepted, false if one of its function pointers
 *  was NULL.
 * 
 *  @remarks It is thoroughly advised not to change Monga's allocator once any allocations
 *  have been made (they can be made from within any Monga procedure), since calls to Mon_Free
 *  might (and most likely will) be made to objects allocated with the previous allocator.
 */ 
MON_PUBLIC bool  MON_CALL Mon_SetAllocator(Mon_Allocator allocator);

MON_PUBLIC void* MON_CALL Mon_Alloc(size_t s);
MON_PUBLIC void* MON_CALL Mon_AllocZero(size_t n, size_t s);
MON_PUBLIC void* MON_CALL Mon_Realloc(void* oldMem, size_t s);
MON_PUBLIC void  MON_CALL Mon_Free(void* mem);

C_LINKAGE_END

#endif // ALLOC_H