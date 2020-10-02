#include <stdio.h>
#include <stdbool.h>

#include "lex.h"
#include "parser.h"
#include "error.h"

extern FILE* yyin;

/*
	External LEX and YACC procedures
*/

int yylex();
int yyparse();

int main(int argc, char* argv[]) {
	int tk;
	bool usedCustomInput = false;

	if (argc > 1) {
		yyin = fopen(argv[1], "r");

		if (yyin == NULL) {
			Mon_Fatal(MON_ERR_FILENOTFOUND);
		}

		usedCustomInput = true;
	}

	do {
		tk = yylex();

		if (tk == MON_TK_EOF) {
			break;
		}

		printf("%s", Mon_GetTokenName(tk));

		if (tk == MON_TK_LIT_INT) {
			printf(" %ld\n", tkval.integer);
		} else if (tk == MON_TK_LIT_FLOAT) {
			printf(" %.3f\n", tkval.real);
		} else if (tk == MON_TK_IDENTIFIER) {
			printf(" %s\n", tkval.identifier.name);
		} else {
			printf("\n");
		}

	} while (true);

	if (usedCustomInput) {
		fclose(yyin);
	}
}