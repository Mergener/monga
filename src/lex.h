#ifndef LEX_H
#define LEX_H

#include <stdio.h>
#include <limits.h>

#include <monga.tab.h>

#define MON_TK_EOF 0

typedef enum yytokentype Mon_TkType;
typedef YYSTYPE Mon_TkVal;

extern Mon_TkVal tkval;

extern int yylex();

const char* Mon_GetTokenName(Mon_TkType tkType);

#endif // LEX_H