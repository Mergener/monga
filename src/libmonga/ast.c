#include "mon_ast.h"

#include <stdarg.h>
#include <stdbool.h>

#include "ast_dump.h"
#include "ast_xml.h"
#include "mon_alloc.h"
#include "mon_error.h"
#include "mon_debug.h"

Mon_RetCode Mon_DumpAst(const Mon_Ast* ast, 
                        FILE* outputStream, 
                        Mon_AstDumpFormat fmt,
                        Mon_AstDumpFlags flags) {

    if (ast == NULL ||
        outputStream == NULL) {
           return MON_ERR_BAD_ARG;
    }

    AstDumpContext ctx;
    ctx.ioErr = false;
    ctx.indentLevel = 0;
    ctx.outFile = outputStream;
    ctx.dumpFlags = flags;

    switch (fmt) {
        case MON_ASTDUMP_XML:
            AstDumpXml(&ctx, ast);
            break;

        default:
            return MON_ERR_BAD_ARG;
    }

    fflush(ctx.outFile);

    if (!ctx.ioErr) {
        return MON_SUCCESS;
    } else {
        return MON_ERR_IO;
    }
}

void Mon_AstFinalize(Mon_Ast* ast) {
    if (ast != NULL) {
        MON_VECTOR_FOREACH(&ast->defsVector, Mon_AstDef*, def,
            Mon_AstDefDestroy(def, true);
        );
    }
    if (ast->moduleName != NULL) {
        Mon_Free(ast->moduleName);
    }
    Mon_VectorFinalize(&ast->defsVector);
}