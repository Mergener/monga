#ifndef STRUTILS_H
#define STRUTILS_H

#include "mon_defines.h"

#include <stdbool.h>
#include <stdlib.h>

/**
 *  Creates a copy of a string.
 *
 *  @param s The original string.
 *  @param len The original string length (does not cont trailing '\0')
 *  
 *  @return A pointer to the new Mon_Alloc allocated and copied string or
 *  NULL if allocation failed.
 * 
 *  @remarks Ignores any '\0's of the original string. Does append one
 *  at the end of the returned string, however.
 */
MON_PRIVATE char* DuplicateString(const char* s, size_t len);

#endif // STRUTILS_H