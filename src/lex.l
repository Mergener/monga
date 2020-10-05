%{
#include "mon_lex.h"

#include <stdlib.h>
#include <string.h>

#include "mon_alloc.h"

#include "grammar.tab.h"

typedef YYSTYPE Mon_TkVal;
extern Mon_TkVal yylval;

static Mon_TkVal MakeIdentifier(const char* name, size_t length);

%}

%option noyywrap
%option noinput
%option nounput

%%

[\t\n ]+ ;

"as"             { return MON_TK_AS; }
"else"           { return MON_TK_ELSE; }
"function"       { return MON_TK_FUNCTION; }
"if"             { return MON_TK_IF; }
"new"            { return MON_TK_NEW; }
"type"           { return MON_TK_TYPE; }
"var"            { return MON_TK_VAR; }
"return"         { return MON_TK_RETURN; }
"while"          { return MON_TK_WHILE; }

"=="             { return MON_TK_OP_EQ; }
">="             { return MON_TK_OP_GE; }
"<="             { return MON_TK_OP_LE; }
"&&"             { return MON_TK_OP_AND; }
"||"             { return MON_TK_OP_OR; }

[a-zA-Z][a-zA-Z_0-9]* {
	yylval = MakeIdentifier(yytext, yyleng);

	return MON_TK_IDENTIFIER;
}

[0-9]+(\.[0-9]+)?[eE][+-]?[0-9]+ {
	yylval.real = strtof(yytext, NULL);
	return MON_TK_LIT_FLOAT;
}

[0-9]+ {
	yylval.integer = strtol(yytext, NULL, 10);
	return MON_TK_LIT_INT;
}

0x[0-9a-fA-F]+ {
	yylval.integer = strtol(yytext, NULL, 16);
	return MON_TK_LIT_INT;
}

[0-9]+(\.[0-9]+) {
	yylval.real = strtof(yytext, NULL);
	return MON_TK_LIT_FLOAT;
}

. {
	return *yytext;
}

%%

static Mon_TkVal MakeIdentifier(const char* name, size_t length) {
	Mon_TkVal ret;

	char* idName = Mon_Alloc(length + 1);

	memcpy(idName, name, length);
	idName[length] = '\0';

	ret.identifier.name = idName;
	ret.identifier.length = length;

	return ret;
}

