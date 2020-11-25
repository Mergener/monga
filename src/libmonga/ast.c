#include "mon_ast.h"
#include "ast_private.h"

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "ast_dump.h"
#include "ast_xml.h"
#include "mon_alloc.h"
#include "mon_error.h"
#include "mon_debug.h"
#include "strutils.h"

static const char* s_UnnamedModuleName = "";

Mon_Ast* Mon_AstNew(const char* moduleName) {
    Mon_Ast* ret = Mon_Alloc(sizeof(*ret));
    if (ret == NULL) {
        return NULL;
    }

    if (moduleName != NULL) {
        ret->moduleName = DuplicateString(moduleName, strlen(moduleName));
        if (ret->moduleName == NULL) {
            Mon_Free(ret);
            return NULL;
        }
    } else {
        ret->moduleName = (char*)s_UnnamedModuleName; // No problem dropping the const here.
    }

    ret->astState = MON_ASTSTATE_NONE;
    Mon_DefGroupInit(&ret->definitions);
    Mon_VectorInit(&ret->semantic.usedTypes);
    ret->semantic.hasEntryPointFunction = false;

    return ret;
}

Mon_RetCode Mon_AstAddDefinition(Mon_Ast* ast, Mon_AstDef* definition) {
    MON_CANT_BE_NULL(ast);
    MON_CANT_BE_NULL(definition);

    Mon_RetCode ret = Mon_DefGroupReg(&ast->definitions, definition);
    if (ret != MON_SUCCESS) {
        return ret;
    }

    if (ast->astState == MON_ASTSTATE_SEM_ERR ||
        ast->astState == MON_ASTSTATE_SEM_OK) {
        ast->astState = MON_ASTSTATE_SYNTAX_OK;
    }

    return MON_SUCCESS;
}

Mon_AstState Mon_AstGetState(const Mon_Ast* ast) {
    MON_CANT_BE_NULL(ast);

    return ast->astState;
}

const Mon_DefGroup* Mon_AstGetDefinitions(const Mon_Ast* ast) {
    MON_CANT_BE_NULL(ast);

    return &ast->definitions;
}

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

void Mon_AstDestroy(Mon_Ast* ast) {
    if (ast != NULL) {
        MON_DEFGROUP_FOREACH(&ast->definitions, def,
            Mon_AstDefDestroy(def, true);
        );
    }
    if (ast->moduleName != NULL) {
        Mon_Free(ast->moduleName);
    }
    Mon_DefGroupFinalize(&ast->definitions);
    Mon_Free(ast);
}