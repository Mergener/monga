// The following is needed since the YACC generated header file must
// be aware of types defined in these headers.
%code requires {
#include "mon_ast.h"
#include "mon_stack.h"
}

%{

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "grammar.tab.h"

#define THROW_IF_ALLOC_FAIL(var)

int yylex();
void yyerror(const char* s);

/** 
 *  Pointer to the user provided AST object to be filled.  
 *
 *  See parser.c for detailed information. 
 */
extern Mon_Ast* mon_TargetAst;

extern bool mon_DumpReduces;

static void DumpReduce(const char* fmt, ...);

%}

%token MON_TK_IDENTIFIER
%token MON_TK_TYPE
%token MON_TK_VAR
%token MON_TK_FUNCTION
%token MON_TK_IF
%token MON_TK_ELSE
%token MON_TK_WHILE
%token MON_TK_RETURN
%token MON_TK_NEW
%token MON_TK_AS
%token MON_TK_LIT_INT
%token MON_TK_LIT_FLOAT
%token MON_TK_OP_ADD
%token MON_TK_OP_SUB
%token MON_TK_OP_MUL
%token MON_TK_OP_DIV
%token MON_TK_OP_EQ
%token MON_TK_OP_NE
%token MON_TK_OP_GT
%token MON_TK_OP_GE
%token MON_TK_OP_LE
%token MON_TK_OP_LT
%token MON_TK_OP_AND
%token MON_TK_OP_OR
%token MON_TK_OP_NOT

%union {
    struct {
            const char* name;
            unsigned    length;
    } identifier;
    
    long   integer;
    double real;

    Mon_Ast* ast;

    Mon_AstDef*       defNode;
    Mon_AstBlock*     blockNode;
    Mon_AstTypeDef*   typeDefNode;
    Mon_AstVarDef*    varDefNode;
    Mon_AstFuncDef*   funcDefNode;
    Mon_AstStatement* statementNode;
    Mon_AstParam*     paramNode;
}

%type <identifier>  MON_TK_IDENTIFIER type opt_ret_type
%type <blockNode>   block opt_else
%type <defNode>     definition definitions   
%type <varDefNode>  def_variable variable_defs opt_variable_defs
%type <typeDefNode> def_type
%type <funcDefNode> def_function
%type <paramNode>   parameter parameters opt_parameters
%type <statement>   statement statements opt_statements

%%

module: 
    definitions {
        DumpReduce("module r1");

        mon_TargetAst->rootDefinition = $1;
    }

    | /* nothing */ {
        DumpReduce("module r2");

        mon_TargetAst->rootDefinition = NULL;
    }
;

definitions: 
    definition {
        DumpReduce("definitions r1");

        $$ = $1;
    }

    | definitions definition {
        DumpReduce("definitions r2");

        $$ = $1;
        $$->next = $2;
    }
;

definition: 
    def_variable { 
        DumpReduce("definition r1");

        $$ = Mon_AstDefNewVar($1); 

        THROW_IF_ALLOC_FAIL($$);
    }

    | def_function { 
        DumpReduce("definition r2");

        $$ = Mon_AstDefNewFunc($1); 

        THROW_IF_ALLOC_FAIL($$);
    }

    | def_type { 
        DumpReduce("definition r3");

        $$ = Mon_AstDefNewType($1); 

        THROW_IF_ALLOC_FAIL($$);
    }
;

def_variable: 
    MON_TK_VAR MON_TK_IDENTIFIER ':' type ';' {
        DumpReduce("def_variable r1");

        $$ = Mon_AstVarDefNew($2.name, $2.length, $4.name, $4.length);

        THROW_IF_ALLOC_FAIL($$);
    }
;

opt_variable_defs: 
    /* nothing */ {
        DumpReduce("opt_variable_defs r1");

        $$ = NULL;
    }

    | variable_defs {
        DumpReduce("opt_variable_defs r2");

        $$ = $1;
    }
;

variable_defs: 
    def_variable {
        DumpReduce("variable_defs r1");

        $$ = $1;
    }

    | variable_defs def_variable {
        DumpReduce("variable_defs r2");

        $$ = $1;
        $$->next = $1;
    }
;

type: 
    MON_TK_IDENTIFIER {
        DumpReduce("type r1");

        $$ = $1;
    }
; 

def_type: 
    MON_TK_TYPE MON_TK_IDENTIFIER '=' typedesc {
        DumpReduce("def_type r1");
    }
;

typedesc: 
    MON_TK_IDENTIFIER {
        DumpReduce("typedesc r1");
    }

    | '[' typedesc ']' {
        DumpReduce("typedesc r2");
    }

    | '{' fields '}' {
        DumpReduce("typedesc r3");
    }
;

field: 
    MON_TK_IDENTIFIER ':' type ';' {
        DumpReduce("field r1");
    }
;

fields: 
    field {
        DumpReduce("fields r1");
    }

    | fields field {
        DumpReduce("fields r2");
    }
;

def_function: 
    MON_TK_FUNCTION MON_TK_IDENTIFIER '(' opt_parameters ')' opt_ret_type block {
        DumpReduce("def_function r1");

        $$ = Mon_AstFuncDefNew(
            $2.name,
            $2.length,
            $6.name,
            $6.length,
            $4, $7
        );

        THROW_IF_ALLOC_FAIL($$);
    }
;

opt_ret_type: 
    /* nothing */ {
        DumpReduce("opt_ret_type r1");

        $$.name = NULL;
        $$.length = 0;
    }

    | ':' type {
        DumpReduce("opt_ret_type r2");

        $$ = $2;
    }
;

parameters: 
    parameter {
        DumpReduce("parameters r1");

        $$ = $1;
    }

    | parameters ',' parameter {
        DumpReduce("parameters r2");

        $$ = $1;
        $1->next = $3;
    }
;

opt_parameters: 
    /* nothing */ {
        DumpReduce("opt_parameters r1");

        $$ = NULL;
    }

    | parameters {
        DumpReduce("opt_parameters r1");

        $$ = $1;
    }
;

parameter: 
    MON_TK_IDENTIFIER ':' type {
        DumpReduce("parameter r1");

        $$ = Mon_AstParamNew($1.name, $1.length, $3.name, $3.length);

        THROW_IF_ALLOC_FAIL($$);
    }
;

block: 
    '{' opt_variable_defs opt_statements '}' {
        DumpReduce("block r1");

        $$ = Mon_AstBlockNew($1, $2);

        THROW_IF_ALLOC_FAIL($$);
    }
;

statement: 
    MON_TK_IF cond block opt_else {
        DumpReduce("statement r1");
    }

    | MON_TK_WHILE cond block {
        DumpReduce("block r2");
    }

    | lvalue '=' exp ';' {
        DumpReduce("block r3");
    }

    | MON_TK_RETURN opt_exp ';' {
        DumpReduce("block r4");
    }

    | call ';' {
        DumpReduce("block r5");
    }

    | '@' exp ';' {
        DumpReduce("block r6");
    }

    | block {
        DumpReduce("block r7");
    }
;

opt_else: 
    /* nothing */ {
        DumpReduce("opt_else r1");

        $$ = NULL;
    }

    | MON_TK_ELSE block {
        DumpReduce("opt_else r2");

        $$ = $2;
    }
;

statements: 
    statement {
        DumpReduce("statements r1");
    }
    
    | statements statement {
        DumpReduce("statements r2");
    }
;

opt_statements: 
    /* nothing */  {
        DumpReduce("opt_statements r1");
    }

    | statements {
        DumpReduce("opt_statements r2");
    }
;

var: 
    MON_TK_IDENTIFIER {
        DumpReduce("var r1");
    }

    | var '.' MON_TK_IDENTIFIER {
        DumpReduce("var r2");
    }
;

numeral: 
    MON_TK_LIT_FLOAT {
        DumpReduce("numeral r1");
    }

    | MON_TK_LIT_INT {
        DumpReduce("numeral r2");
    }
;

opt_exp: 
    /* nothing */ {
        DumpReduce("opt_exp r1");
    }

    | exp {
        DumpReduce("opt_exp r2");
    }
;

exp: 
    exp_conditional {
        DumpReduce("exp r1");
    }
;

lvalue: 
    var {
        DumpReduce("lvalue r1");
    }

    | exp_primary bracket_exp {
        DumpReduce("lvalue r2");
    }
;

exp_primary: 
    '(' exp ')' {
        DumpReduce("exp_primary r1");
    }

    | numeral {
        DumpReduce("exp_primary r2");
    }

    | lvalue {
        DumpReduce("exp_primary r3");
    }
;

exp_postfix: 
    exp_primary {
        DumpReduce("exp_postfix r1");
    }

    | exp_primary MON_TK_AS type {
        DumpReduce("exp_postfix r2");
    }
;
              
exp_unary: 
    exp_postfix {
        DumpReduce("exp_unary r1");
    }

    | '-' exp_primary {
        DumpReduce("exp_unary r2");
    }
;

exp_multiplicative: 
    exp_unary {
        DumpReduce("exp_multiplicative r1");
    }

    | exp_multiplicative '*' exp_unary {
        DumpReduce("exp_multiplicative r2");
    }

    | exp_multiplicative '/' exp_unary {
        DumpReduce("exp_multiplicative r3");
    }
;

exp_additive: 
    exp_multiplicative {
        DumpReduce("exp_additive r1");
    }

    | exp_additive '+' exp_multiplicative {
        DumpReduce("exp_additive r2");
    }

    | exp_additive '-' exp_multiplicative {
        DumpReduce("exp_additive r3");
    }
;

exp_conditional: 
    exp_additive {
        DumpReduce("exp_conditional r1");
    }

    | cond_primary '?' exp ':' exp_conditional {
        DumpReduce("exp_conditional r2");
    }

    | MON_TK_NEW type opt_bracket_exp {
        DumpReduce("exp_conditional r3");
    }
;

opt_bracket_exp: 
    /* nothing */ {
        DumpReduce("opt_bracket_exp r1");
    }

    | bracket_exp {
        DumpReduce("opt_bracket_exp r2");
    }
;

bracket_exp: 
    '[' exp ']' {
        DumpReduce("bracket_exp r1");
    }
;

cond: 
    cond_or {
        DumpReduce("cond_or r1");
    }
;

cond_primary: 
    '(' cond ')' {
        DumpReduce("cond_primary r1");
    }

    | exp_additive MON_TK_OP_EQ exp_additive {
        DumpReduce("cond_primary r2");
    }

    | exp_additive MON_TK_OP_NE exp_additive {
        DumpReduce("cond_primary r3");
    }

    | exp_additive MON_TK_OP_LE exp_additive {
        DumpReduce("cond_primary r4");
    }

    | exp_additive MON_TK_OP_GE exp_additive {
        DumpReduce("cond_primary r5");
    }

    | exp_additive '>' exp_additive {
        DumpReduce("cond_primary r6");
    }

    | exp_additive '<' exp_additive {
        DumpReduce("cond_primary r7");
    }
;

cond_not: 
    cond_primary {
        DumpReduce("cond_not r1");
    }

    | '!' '(' cond ')' {
        DumpReduce("cond_not r2");
    }
;

cond_and: 
    cond_not {
        DumpReduce("cond_and r1");
    }

    | cond_and MON_TK_OP_AND cond_not {
        DumpReduce("cond_and r2");
    }
;

cond_or: 
    cond_and {
        DumpReduce("cond_or r1");
    }

    | cond_or MON_TK_OP_OR cond_and {
        DumpReduce("cond_or r2");
    }
;

call: 
    MON_TK_IDENTIFIER '(' opt_exps ')'  {
        DumpReduce("call r1");
    }
;

exps: 
    exp  {
        DumpReduce("exps r1");
    }

    | exps ',' exp  {
        DumpReduce("exps r2");
    }
;

opt_exps: 
    /* nothing */  {
        DumpReduce("opt_exps r1");
    }

    | exps  {
        DumpReduce("opt_exps r2");
    }
;

%%

void yyerror(const char* s) {
    fprintf(stderr, "%s\n", s);
}

static void DumpReduce(const char* fmt, ...) {
    if (mon_DumpReduces) {
        va_list args;
        va_start(args, fmt);

        vprintf(fmt, args);
        printf("\n");

        va_end(args);
    }
}