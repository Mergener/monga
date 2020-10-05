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
 *	Prints the specified format string with formatted arguments to a dump context's out file
 *	and checks for IO errors. If any is found, ctx->ioErr is set to true.
 *
 * 	@param ctx The dumping context.
 * 	@param fmt The format string.
 * 
 * 	@return True if no IO errors occurred, false otherwise.
 */ 
MON_PRIVATE bool Dumpf(AstDumpContext* ctx, const char* fmt, ...);

#endif // AST_DUMP_H