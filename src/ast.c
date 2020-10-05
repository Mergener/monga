#include "ast.h"

#include <stdarg.h>
#include <stdbool.h>

#include "error.h"

typedef struct {

	/** The file being written to. */
	FILE* outFile;

	/** The current indentation level. */
	int indentLevel;

	/** True if an IO error happened. */
	bool ioErr;

	Mon_AstDumpFlags dumpFlags;

} DumpingContext;

static void Dumpf(DumpingContext* ctx, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

	bool prettyPrint = (ctx->dumpFlags & MON_ASTDUMP_FLAGS_PRETTYPRINT) != 0;
	
	if (prettyPrint) {
		for (int i = ctx->indentLevel; i > 0; --i)	{
			if (fprintf(ctx->outFile, "\t") < 0) {
				ctx->ioErr = true;
			}
		}
	}

	if (vfprintf(ctx->outFile, fmt, args) < 0) {
		ctx->ioErr = true;
	}

	if (prettyPrint) {
		fprintf(ctx->outFile, "\n");
	}

    va_end(args);
}

/////////////////////////////////////////////
// XML Dumping
/////////////////////////////////////////////

static void XmlDumpFuncNode(DumpingContext* ctx, const Mon_AstFuncDef* funcDef) {
	Dumpf(ctx, "<FunctionDefinition>");
	ctx->indentLevel++;

	Dumpf(ctx, "<Name>%s</Name>", funcDef->funcName);

	ctx->indentLevel--;
	Dumpf(ctx, "</FunctionDefinition>");
}

static void XmlDumpVarNode(DumpingContext* ctx, const Mon_AstVarDef* varDef) {
	Dumpf(ctx, "<VariableDeclaration>");
	ctx->indentLevel++;

	Dumpf(ctx, "<Name>%s</Name>", varDef->varName);
	Dumpf(ctx, "<Type></Type>");

	ctx->indentLevel--;
	Dumpf(ctx, "</VariableDeclaration>");
}

static void XmlDumpTypeNode(DumpingContext* ctx, const Mon_AstTypeDef* typeDef) {
	Dumpf(ctx, "<TypeDefinition>");
	ctx->indentLevel++;

	Dumpf(ctx, "<Name>%s</Name>", typeDef->typeName);

	ctx->indentLevel--;
	Dumpf(ctx, "</TypeDefinition>");
}

static void XmlDumpDefNode(DumpingContext* ctx, const Mon_AstDef* defNode) {
	switch (defNode->defKind) {
		case MON_AST_DEF_FUNC:
			XmlDumpFuncNode(ctx, &defNode->definition.function);
			break;

		case MON_AST_DEF_VAR:
			XmlDumpVarNode(ctx, &defNode->definition.variable);			
			break;

		case MON_AST_DEF_TYPE:
			XmlDumpTypeNode(ctx, &defNode->definition.type);
			break;

		default:
			break;
	}
}

static void DumpXml(DumpingContext* ctx, const Mon_Ast* ast) {
	// Walk through all definition nodes and dump each of them
	const Mon_AstDef* def = ast->rootDefinition;

	Dumpf(ctx, "<Module>");
	ctx->indentLevel++;

	while (def != NULL) {
		XmlDumpDefNode(ctx, def);

		def = def->next;
	}

	ctx->indentLevel--;
	Dumpf(ctx, "</Module>");
}

/////////////////////////////////////////////

Mon_RetCode Mon_DumpAst(const Mon_Ast* ast, 
				        FILE* outputStream, 
				        Mon_AstDumpFormat fmt,
				        Mon_AstDumpFlags flags) {

	if (ast == NULL ||
	    outputStream == NULL) {
		   return MON_ERR_BAD_ARG;
	}

	DumpingContext ctx;
	ctx.indentLevel = 0;
	ctx.outFile = outputStream;
	ctx.dumpFlags = flags;

	switch (fmt) {
		case MON_ASTDUMP_XML:
			DumpXml(&ctx, ast);
			break;

		default:
			return MON_ERR_BAD_ARG;
	}

	return MON_SUCCESS;
}

void Mon_AstFinalize(Mon_Ast* ast) {

}