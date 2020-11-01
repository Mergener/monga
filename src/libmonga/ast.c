#include "mon_ast.h"

#include <stdarg.h>
#include <stdbool.h>

#include "ast_dump.h"
#include "ast_xml.h"
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
    Mon_VectorFinalize(&ast->defsVector);
}

void Mon_InitializeAstVisitor(Mon_AstVisitor* vis) {
    MON_CANT_BE_NULL(vis);
    
    vis->blockVisitor = NULL;
    vis->callVisitor = NULL;
    vis->condVisitor = NULL;
    vis->defVisitor = NULL;
    vis->expVisitor = NULL;
    vis->fieldVisitor = NULL;
    vis->funcDefVisitor = NULL;
    vis->paramVisitor = NULL;
    vis->stmtVisitor = NULL;
    vis->typeDefVisitor = NULL;
    vis->typeDescVisitor = NULL;
    vis->varDefVisitor = NULL;
    vis->varVisitor = NULL;
}

typedef struct {

    Mon_Ast* ast;
    const Mon_AstVisitor* visitor;
    void* userCtx;
    bool bottomUp;
    bool topDown;

} VisitContext;

bool Mon_VisitAst(Mon_Ast* ast, 
                  const Mon_AstVisitor* visitor, 
                  void* context,
                  Mon_AstVisitDirection direction) {

    MON_CANT_BE_NULL(ast);
    MON_CANT_BE_NULL(visitor);
    
    VisitContext visitContext;
    visitContext.ast = ast;
    visitContext.bottomUp = (direction & MON_ASTVIS_BOTTOMUP) != 0;
    visitContext.topDown = (direction & MON_ASTVIS_TOPDOWN) != 0;
    visitContext.userCtx = context;
    visitContext.visitor = visitor;

    return VisitRoot(&visitContext);
}

static bool VisitRoot(VisitContext* ctx) {
    MON_VECTOR_FOREACH(&ctx->ast->defsVector, Mon_AstDef*, def, 
        if (ctx->topDown && ctx->visitor->defVisitor != NULL) {
            if (!ctx->visitor->defVisitor(ctx->userCtx, def, MON_ASTVIS_TOPDOWN)) {
                return false;
            }
        }

        if (!VisitDef(ctx, def)) {
            return false;
        }

        if (ctx->bottomUp && ctx->visitor->defVisitor != NULL) {
            if (!ctx->visitor->defVisitor(ctx->userCtx, def, MON_ASTVIS_BOTTOMUP)) {
                return false;
            }
        }
    );

    return true;
}

static bool VisitDef(VisitContext* ctx, Mon_AstDef* node) {
    switch (node->defKind) {

        case MON_AST_DEF_VAR:
        
            break;

        case MON_AST_DEF_FUNC:
            break;

        case MON_AST_DEF_TYPE:
            break;
    }
}