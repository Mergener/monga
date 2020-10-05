#ifndef LEX_H
#define LEX_H

#include "defines.h"

#include <stdio.h>
#include <limits.h>

#define MON_TK_EOF 0

typedef int Mon_TkType;

/**
 * Dumps all tokens found in an input file to another output file. 
 * 
 * Each token is dumped in a separate line. Each token is written by its 
 * token name (the same returned by Mon_GetTokenName). If the token is 
 * a literal or an identifier, its parsed value will also be printed in the same line
 * as its token name (separated by a single space). 
 * 
 * Float literals are printed with 3 decimal digits after the dot.
 * 
 * @param inputFile The file to be scanned by the lexer.
 * @param outputFile The file to dump the tokens to.
 */
MON_PUBLIC void MON_CALL Mon_DumpLex(FILE* inputFile, FILE* outputFile);

/**
 * Returns a human readable string representation of a token type.
 * 
 * @param tkType The token to get a string representation of.
 * 
 * @return If the specified token is a token defined in the yytokentype enum,
 * the returned string is simply a lowercase version of its enum name
 * without the MON_TK_ prefix.
 * 
 * If the specified token is a value between 0-255, the returned string is
 * simply a null-terminated string with the ASCII equivalent character of
 * the specified token type.
 * 
 * If the specified type doesn't match any of the descriptions above, the returned
 * value is "unknown".
 */
MON_PUBLIC const char* MON_CALL Mon_GetTokenName(Mon_TkType tkType);

#endif // LEX_H