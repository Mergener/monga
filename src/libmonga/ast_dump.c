#include "ast_dump.h"

#include <stdio.h>

bool Dumpf(AstDumpContext* ctx, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    bool prettyPrint = (ctx->dumpFlags & MON_ASTDUMP_FLAGS_PRETTYPRINT) != 0;
    
    if (prettyPrint) {
        for (int i = ctx->indentLevel; i > 0; --i)    {
            if (fprintf(ctx->outFile, "\t") < 0) {
                ctx->ioErr = true;
                return false;
            }
        }
    }

    if (vfprintf(ctx->outFile, fmt, args) < 0) {
        ctx->ioErr = true;
        return false;
    }

    if (prettyPrint) {
        if (fprintf(ctx->outFile, "\n") < 0) {
            ctx->ioErr = true;
            return false;
        }
    }

    va_end(args);

    return true;
}