#include "montests.h"

#include "mon_parser.h"
#include "mon_ast.h"
#include "mon_sem.h"

static const char* s_TestableFiles[] = {
	"tests/ast_dump_cases/factorial",
	"tests/ast_dump_cases/empty",
	"tests/ast_dump_cases/ok_case1",
	"tests/ast_dump_cases/ok_case2",
	"tests/reduce_dump_cases/factorial",
	"tests/reduce_dump_cases/linkedlist",
	"tests/reduce_dump_cases/ok_case1",
	"tests/reduce_dump_cases/vector3"
};

static Mon_Ast* ConstructSampleAst() {
	Logf("Generating sample AST...\n");

	Mon_Ast* ast = Mon_AstNew("testAST");

	// Create a variable:
	Mon_AstVarDef* varDef = Mon_AstVarDefNew("xyz", 3, "int", 3);
	Mon_AstStatement* varDefStmt = Mon_AstStatementNewVarDef(varDef);

	// Create an assignment statement:
	Mon_AstVar* lvalue = Mon_AstVarNewDirect("xyz");
	Mon_Literal literal;
	literal.integer = 5;
	literal.literalKind = MON_LIT_INT;
	Mon_AstExp* rvalue = Mon_AstExpNewLiteral(literal);
	Mon_AstStatement* assignment = Mon_AstStatementNewAssignment(lvalue, rvalue);

	Mon_Vector statements;
	Mon_VectorInit(&statements);
	Mon_VectorPush(&statements, varDefStmt);
	Mon_VectorPush(&statements, assignment);

	Mon_AstBlock* block = Mon_AstBlockNew(statements);

	Mon_Vector funcParams;
	Mon_VectorInit(&funcParams);

	Mon_AstFuncDef* funcDef = Mon_AstFuncDefNew("foo", 3, "int", 3, funcParams, block);
	Mon_AstDef* firstDef = Mon_AstDefNewFunc(funcDef);

	Mon_AstAddDefinition(ast, firstDef);

	Logf("Generated sample AST:\n");
	Mon_DumpAst(ast, g_LogFileStream, MON_ASTDUMP_XML, MON_ASTDUMP_FLAGS_PRETTYPRINT);

	return ast;
}

static void AstLeakTest() {
	int initialAllocCount = GetAllocCount();
	Mon_Ast* ast = ConstructSampleAst(&ast);

	Mon_AstDestroy(ast);

	int finalAllocCount = GetAllocCount();
	MON_ASSERT(finalAllocCount == initialAllocCount,
		"final number of allocations must be equal to the initial count. (expected %d, got %d)",
		initialAllocCount, finalAllocCount);
}

static void ParseLeakTest() {

	int count = sizeof(s_TestableFiles)/sizeof(*s_TestableFiles);

	for (int i = 0; i < count; ++i) {
		Mon_Ast* ast = Mon_AstNew("ast");

		FILE* f = fopen(s_TestableFiles[i], "r");
		if (f == NULL) {
			continue;
		}
		Logf("Parsing file %s\n", s_TestableFiles[i]);

		int initial = GetAllocCount();

		// We are not testing the outputs of a parsing here, but merely 
		// checking the existence of memory leaks.
		Mon_Parse(f, ast, MON_PARSEFLAGS_NONE);		

		Mon_AstDestroy(ast);

		int finalAlloc = GetAllocCount();
		MON_ASSERT(finalAlloc == initial,
			"in parse test for file %s, final number of allocations must be equal to the intial count. (expected %d, got %d)",
			s_TestableFiles[i], initial, finalAlloc);
		
		fclose(f);
	}
}

static void SemanticLeakTest() {
	int count = sizeof(s_TestableFiles)/sizeof(*s_TestableFiles);

	for (int i = 0; i < count; ++i) {
		Mon_Ast* ast = Mon_AstNew("ast");

		FILE* f = fopen(s_TestableFiles[i], "r");
		if (f == NULL) {
			continue;
		}
		Logf("Parsing file %s\n", s_TestableFiles[i]);

		int initial = GetAllocCount();

		// We are not testing the outputs of a parsing here, but merely 
		// checking the existence of memory leaks.
		Mon_Parse(f, ast, MON_PARSEFLAGS_NONE);	

		Mon_SemAnalyse(ast, g_LogFileStream);

		Mon_AstDestroy(ast);

		int finalAlloc = GetAllocCount();
		MON_ASSERT(finalAlloc == initial,
			"in semantic test for file %s, final number of allocations must be equal to the intial count. (expected %d, got %d)",
			s_TestableFiles[i], initial, finalAlloc);
		
		fclose(f);
	}
}

static Test s_AstTests[] = {
	{ "Ast Memory Leak Test", AstLeakTest },
	{ "Parse Leak Test", ParseLeakTest },
	{ "Semantic Leak Test", SemanticLeakTest }
};

void RunASTTests() {
    printf("-> Starting AST tests.\n");
	RunTests(s_AstTests,  sizeof(s_AstTests) / sizeof(*s_AstTests));
}