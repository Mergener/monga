#ifndef AST_DUMP_H
#define AST_DUMP_H

#include "mon_defines.h"

#include <stdarg.h>
#include <stdbool.h>

#include "mon_ast.h"

/** 
 *	Macro that calls Dumpf, checks for its return value and returns if
 *	it returned false.
 */
#define DUMPF_OR_STOP(ctx, ...) \
	if (!Dumpf(ctx, __VA_ARGS__)) \
	{ \
		return; \
	}

typedef struct {

	/** The file being written to. */
	FILE* outFile;

	/** The current indentation level. */
	int indentLevel;

	/** True if an IO error happened. */
	bool ioErr;

	Mon_AstDumpFlags dumpFlags;

} AstDumpContext;

/**
 *	
 */ 
MON_PRIVATE bool Dumpf(AstDumpContext* ctx, const char* fmt, ...);

#endif // AST_DUMP_H