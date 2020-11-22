%{
#include "mon_lex.h"

#include <stdlib.h>
#include <string.h>

#include "mon_alloc.h"
#include "strutils.h"

#include "grammar.tab.h"

typedef YYSTYPE Mon_TkVal;
extern Mon_TkVal yylval;

int Mon_TkLine;
int Mon_TkColumn;

static Mon_TkVal MakeIdentifier(const char* name, size_t length);

%}

%option noyywrap
%option noinput
%option nounput

%%

"#".* {
    // Ignore (comment)
}

\"([^\\\"]|\\.)*\" {
    Mon_TkColumn += yyleng;

    yylval.literal.string.arr = DuplicateString(yytext + sizeof(char), yyleng - 2*sizeof(char));
    yylval.literal.string.len = yyleng - 2;
    yylval.literal.literalKind = MON_LIT_STR;

    return MON_TK_LIT_STRING;
}

\n {
    Mon_TkColumn = 1;
    Mon_TkLine++;
}

[\t ]+ {
    Mon_TkColumn += yyleng;
}

"extern" {
    Mon_TkColumn += yyleng;
    return MON_TK_EXTERN;
}

"break" {
    Mon_TkColumn += yyleng;
    return MON_TK_BREAK;
}

"continue" {
    Mon_TkColumn += yyleng;
    return MON_TK_CONTINUE;
}

"len" {
    Mon_TkColumn += yyleng;
    return MON_TK_LEN;
}

"as" {
    Mon_TkColumn += yyleng;
    return MON_TK_AS;
}

"else" {
    Mon_TkColumn += yyleng;
    return MON_TK_ELSE;
}

"function" {
    Mon_TkColumn += yyleng;
    return MON_TK_FUNCTION;
}

"if" {
    Mon_TkColumn += yyleng;
    return MON_TK_IF;
}

"new" {
    Mon_TkColumn += yyleng;
    return MON_TK_NEW;
}

"type" {
    Mon_TkColumn += yyleng;
    return MON_TK_TYPE;
}

"var" {
    Mon_TkColumn += yyleng;
    return MON_TK_VAR;
}

"return" {
    Mon_TkColumn += yyleng;
    return MON_TK_RETURN;
}

"while" {
    Mon_TkColumn += yyleng;
    return MON_TK_WHILE;
}
"==" {
    Mon_TkColumn += yyleng;
    return MON_TK_OP_EQ;
}

">=" {
    Mon_TkColumn += yyleng;
    return MON_TK_OP_GE;
}

"<=" {
    Mon_TkColumn += yyleng;
    return MON_TK_OP_LE;
}

"&&" {
    Mon_TkColumn += yyleng;
    return MON_TK_OP_AND;
}

"||" {
    Mon_TkColumn += yyleng;
    return MON_TK_OP_OR;
}

"<<" {
    Mon_TkColumn += yyleng;
    return MON_TK_OP_SHL;
}

">>" {
    Mon_TkColumn += yyleng;
    return MON_TK_OP_SHR;
}

"null" {
    Mon_TkColumn += yyleng;
    
    return MON_TK_NULL;
}

[a-zA-Z_][a-zA-Z_0-9]* {
    Mon_TkColumn += yyleng;
    yylval = MakeIdentifier(yytext, yyleng);

    return MON_TK_IDENTIFIER;
}

[0-9]+(\.[0-9]+) {
    Mon_TkColumn += yyleng;
    yylval.literal.real = strtof(yytext, NULL);
    yylval.literal.literalKind = MON_LIT_FLOAT;

    return MON_TK_LIT_FLOAT;
}

[0-9]+(\.[0-9]+)?[eE][+-]?[0-9]+ {
    Mon_TkColumn += yyleng;
    yylval.literal.real = strtof(yytext, NULL);
    yylval.literal.literalKind = MON_LIT_FLOAT;

    return MON_TK_LIT_FLOAT;
}

[0-9]+ {
    Mon_TkColumn += yyleng;
    yylval.literal.integer = strtol(yytext, NULL, 10);
    yylval.literal.literalKind = MON_LIT_INT;

    return MON_TK_LIT_INT;
}

0x[0-9a-fA-F]+ {
    Mon_TkColumn += yyleng;
    yylval.literal.integer = strtol(yytext, NULL, 16);
    return MON_TK_LIT_INT;
}

. {
    Mon_TkColumn += yyleng;
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

