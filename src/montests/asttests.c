#include "montests.h"

#include "mon_parser.h"
#include "mon_ast.h"

static void ConstructSampleAst(Mon_Ast* ast) {
	Logf("Generating sample AST...\n");

	Mon_AstVarDef* varDef = Mon_AstVarDefNew("xyz", 3, "int", 3);

	Mon_Vector varDefs;
	Mon_VectorInit(&varDefs);
	Mon_VectorPush(&varDefs, varDef);

	Mon_AstVar* lvalue = Mon_AstVarNewDirect("xyz");
	Mon_Literal literal;
	literal.integer = 5;
	literal.literalKind = MON_LIT_INT;
	Mon_AstExp* rvalue = Mon_AstExpNewLiteral(literal);

	Mon_AstStatement* assignment = Mon_AstStatementNewAssignment(lvalue, rvalue);

	Mon_Vector statements;
	Mon_VectorInit(&statements);
	Mon_VectorPush(&statements, assignment);

	Mon_AstBlock* block = Mon_AstBlockNew(varDefs, statements);

	Mon_Vector funcParams;
	Mon_VectorInit(&funcParams);

	Mon_AstFuncDef* funcDef = Mon_AstFuncDefNew("foo", 3, "int", 3, funcParams, block);
	Mon_AstDef* firstDef = Mon_AstDefNewFunc(funcDef);

	Mon_VectorInit(&ast->defsVector);
	Mon_VectorPush(&ast->defsVector, firstDef);

	Logf("Generated sample AST:\n");
	Mon_DumpAst(ast, g_LogFileStream, MON_ASTDUMP_XML, MON_ASTDUMP_FLAGS_PRETTYPRINT);
}

static void AstLeakTest() {
	Mon_Ast ast;
	int initialAllocCount = GetAllocCount();

	ConstructSampleAst(&ast);

	Mon_AstFinalize(&ast);

	int finalAllocCount = GetAllocCount();
	MON_ASSERT(finalAllocCount == initialAllocCount,
		"final number of allocations must be equal to the initial count. (expected %d, got %d)",
		initialAllocCount, finalAllocCount);
}

static Test s_AstTests[] = {
	{ "Ast Memory Leak Test", AstLeakTest }
};

void RunASTTests() {
    printf("-> Starting AST tests.\n");
	RunTests(s_AstTests,  sizeof(s_AstTests) / sizeof(*s_AstTests));
}