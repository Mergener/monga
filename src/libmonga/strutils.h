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

/**
 *  Converts a control character (one that would be placed after a '\' in a
 *  string or character literal) to its equivalent ASCII character codepoint.
 *  For example: The character n becomes 10, which is the ASCII for the control
 *  newline character ('\n').
 */
MON_PRIVATE char ConvertControlCharacter(char c);

MON_PRIVATE void ProcessStringLiteral(const char* s, char** outBuf, int* outLen);

#endif // STRUTILS_H