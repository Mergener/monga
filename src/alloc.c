#include "alloc.h"

#include "error.h"

void* Mon_Alloc(size_t size) {
	void* mem = malloc(size);
	if (mem == NULL) {
		Mon_Fatal(MON_ERR_NOMEM);
	}
	return mem;
}