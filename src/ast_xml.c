#include "ast_dump.h"

static void XmlDumpFuncNode(AstDumpContext* ctx, const Mon_AstFuncDef* funcDef) {
	DUMPF_OR_STOP(ctx, "<FunctionDefinition>");
	ctx->indentLevel++;

	DUMPF_OR_STOP(ctx, "<Name>%s</Name>", funcDef->funcName);

	ctx->indentLevel--;
	DUMPF_OR_STOP(ctx, "</FunctionDefinition>");
}

static void XmlDumpVarNode(AstDumpContext* ctx, const Mon_AstVarDef* varDef) {
	DUMPF_OR_STOP(ctx, "<VariableDeclaration>");
	ctx->indentLevel++;

	DUMPF_OR_STOP(ctx, "<Name>%s</Name>", varDef->varName);
	DUMPF_OR_STOP(ctx, "<Type></Type>");

	ctx->indentLevel--;
	DUMPF_OR_STOP(ctx, "</VariableDeclaration>");
}

static void XmlDumpTypeNode(AstDumpContext* ctx, const Mon_AstTypeDef* typeDef) {
	DUMPF_OR_STOP(ctx, "<TypeDefinition>");
	ctx->indentLevel++;

	DUMPF_OR_STOP(ctx, "<Name>%s</Name>", typeDef->typeName);

	ctx->indentLevel--;
	DUMPF_OR_STOP(ctx, "</TypeDefinition>");
}

static void XmlDumpDefNode(AstDumpContext* ctx, const Mon_AstDef* defNode) {
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

void AstDumpXml(AstDumpContext* ctx, const Mon_Ast* ast) {
	// Walk through all definition nodes and dump each of them
	const Mon_AstDef* def = ast->rootDefinition;

	DUMPF_OR_STOP(ctx, "<Module>");
	ctx->indentLevel++;

	while (def != NULL) {
		XmlDumpDefNode(ctx, def);
		if (ctx->ioErr) {
			return;
		}

		def = def->next;
	}

	ctx->indentLevel--;
	DUMPF_OR_STOP(ctx, "</Module>");
}