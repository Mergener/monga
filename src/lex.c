#include "lex.h"

#include <stdbool.h>
#include <stdatomic.h>

extern FILE* yyin;
extern Mon_TkVal yylval;
extern int yylex();

static atomic_bool s_AsciiInitialized = false;
static char s_Ascii[512];

static atomic_bool s_Busy = false;

void Mon_DumpLex(FILE* inputFile, FILE* outputFile) {
	while (s_Busy);
	s_Busy = true;

	yyin = inputFile;

	Mon_TkType tkType;
	do {
		tkType = yylex();

		if (tkType == MON_TK_LIT_FLOAT) {
			fprintf(outputFile, "%s %.3f\n", Mon_GetTokenName(tkType), yylval.real);
		} else if (tkType == MON_TK_LIT_INT) {
			fprintf(outputFile, "%s %ld\n", Mon_GetTokenName(tkType), yylval.integer);
		} else if (tkType == MON_TK_IDENTIFIER) {
			fprintf(outputFile, "%s %s\n", Mon_GetTokenName(tkType), yylval.identifier.name);
		} else {
			fprintf(outputFile, "%s\n", Mon_GetTokenName(tkType));
		}
	} while (tkType != MON_TK_EOF);

	s_Busy = false;
}

const char* Mon_GetTokenName(Mon_TkType tkType) {
	if (!s_AsciiInitialized) {
		for (int i = 0; i < 512; i += 2) {
			s_Ascii[i] = i / 2;
			s_Ascii[i+1] = '\0';
		}
		s_AsciiInitialized = true;
	}

	switch (tkType) {
		case MON_TK_AS:
			return "as";
			
		case MON_TK_IF:
			return "if";

		case MON_TK_VAR:
			return "var";

		case MON_TK_ELSE:
			return "else";

		case MON_TK_NEW:
			return "new";

		case MON_TK_WHILE:
			return "while";

		case MON_TK_RETURN:
			return "return";

		case MON_TK_FUNCTION:
			return "function";

		case MON_TK_TYPE:
			return "type";

		case MON_TK_IDENTIFIER:
			return "identifier";

		case MON_TK_LIT_INT:
			return "lit_int";

		case MON_TK_LIT_FLOAT:
			return "lit_float";

		case MON_TK_OP_ADD:
			return "op_add";

		case MON_TK_OP_SUB:
			return "op_sub";

		case MON_TK_OP_MUL:
			return "op_mul";

		case MON_TK_OP_DIV:
			return "op_div";

		case MON_TK_OP_EQ:
			return "op_eq";

		case MON_TK_OP_NE:
			return "op_ne";

		case MON_TK_OP_GT:
			return "op_gt";

		case MON_TK_OP_GE:
			return "op_ge";

		case MON_TK_OP_LE:
			return "op_le";

		case MON_TK_OP_LT:
			return "op_lt";

		case MON_TK_OP_AND:
			return "op_and";

		case MON_TK_OP_OR:
			return "op_or";

		case MON_TK_OP_NOT:
			return "op_not";

		default:
			if (tkType < UCHAR_MAX) {
				return &s_Ascii[tkType*2];
			} else {
				return "unknown";
			}
	}
}