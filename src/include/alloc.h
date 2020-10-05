#ifndef ALLOC_H
#define ALLOC_H

#include "defines.h"

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
 *	Provides a custom allocator to Monga. The custom allocator's functions
 *	will then be used by Monga's Mon_Alloc, Mon_Realloc and Mon_Free routines.
 * 
 * 	@return True if the allocator was accepted, false if one of its function pointers
 * 	was NULL.
 */ 
MON_PUBLIC bool MON_CALL Mon_SetAllocator(Mon_Allocator allocator);
MON_PUBLIC void* MON_CALL Mon_Alloc(size_t s);
MON_PUBLIC void* MON_CALL Mon_Realloc(void* oldMem, size_t s);
MON_PUBLIC void  MON_CALL Mon_Free(void* mem);

C_LINKAGE_END

#endif // ALLOC_H