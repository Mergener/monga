// The following is needed since the YACC generated header file must
// be aware of types defined in these headers.
%code requires {
#include "mon_ast.h"
#include "mon_stack.h"
}

%{

#include <stdio.h>
#include <stdlib.h>

#include "grammar.tab.h"

#define DEBUGF(...)
//#define DEBUGF(...) printf(__VA_ARGS__); printf("\n")
#define THROW_IF_ALLOC_FAIL(var)

int yylex();
void yyerror(const char* s);

/** 
 *  Pointer to the user provided AST object to be filled.  
 *
 *  See parser.c for detailed information. 
 */
extern Mon_Ast* mon_TargetAst;

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

    Mon_Ast*      ast;
    Mon_AstDef*   defNode;
    Mon_AstParam* paramNode;
}

%type <identifier> MON_TK_IDENTIFIER type
%type <defNode>    definition definitions def_variable def_function def_type
%type <paramNode>  parameter parameters opt_parameters

%%

module: 
    definitions {
        DEBUGF("module r1");

        mon_TargetAst->rootDefinition = $1;
    }

    | /* nothing */ {
        DEBUGF("module r2");

        mon_TargetAst->rootDefinition = NULL;
    }
;

definitions: 
    definition {
        DEBUGF("definitions r1");

        $$ = $1;
    }

    | definitions definition {
        DEBUGF("definitions r2");

        $$ = $1;
        $$->next = $2;
    }
;

definition: 
    def_variable { 
        DEBUGF("definition r1");

        $$ = $1; 
    }

    | def_function { 
        DEBUGF("definition r2");

        $$ = $1; 
    }

    | def_type { 
        DEBUGF("definition r3");

        $$ = $1; 
    }
;

def_variable: 
    MON_TK_VAR MON_TK_IDENTIFIER ':' type ';' {
        DEBUGF("def_variable r1");

        $$ = Mon_AstVarDefNew($2.name, $2.length, $4.name, $4.length);

        THROW_IF_ALLOC_FAIL($$);
    }
;

opt_variable_defs: 
    /* nothing */ {
        DEBUGF("opt_variable_defs r1");
    }

    | variable_defs {
        DEBUGF("opt_variable_defs r2");        
    }
;

variable_defs: 
    def_variable {
        DEBUGF("variable_defs r1");
    }
    | variable_defs def_variable {
        DEBUGF("variable_defs r2");
    }
;

type: MON_TK_IDENTIFIER {
    DEBUGF("type r1");

    $$ = $1;
}
; 

def_type: 
    MON_TK_TYPE MON_TK_IDENTIFIER '=' typedesc {
        DEBUGF("def_type r1");
    }
;

typedesc: 
    MON_TK_IDENTIFIER {
        DEBUGF("typedesc r1");
    }

    | '[' typedesc ']' {
        DEBUGF("typedesc r2");
    }

    | '{' fields '}' {
        DEBUGF("typedesc r3");
    }
;

field: 
    MON_TK_IDENTIFIER ':' type ';' {
        DEBUGF("field r1");
    }
;

fields: 
    field {
        DEBUGF("fields r1");
    }

    | fields field {
        DEBUGF("fields r2");
    }
;

def_function: 
    MON_TK_FUNCTION MON_TK_IDENTIFIER '(' opt_parameters ')' opt_ret_type block {
        DEBUGF("def_function r1");

        $$ = Mon_AstFuncDefNew($2.name, $2.length, $4);

        THROW_IF_ALLOC_FAIL($$);
    }
;

opt_ret_type: 
    /* nothing */ {
        DEBUGF("opt_ret_type r1");
    }

    | ':' type {
        DEBUGF("opt_ret_type r2");
    }
;

parameters: 
    parameter {
        DEBUGF("parameters r1");

        $$ = $1;
    }

    | parameters ',' parameter {
        DEBUGF("parameters r2");

        $$ = $1;
        $1->next = $3;
    }
;

opt_parameters: 
    /* nothing */ {
        DEBUGF("opt_parameters r1");

        $$ = NULL;
    }

    | parameters {
        DEBUGF("opt_parameters r1");

        $$ = $1;
    }
;

parameter: 
    MON_TK_IDENTIFIER ':' type {
        DEBUGF("parameter r1");

        $$ = Mon_AstParamNew($1.name, $1.length, $3.name, $3.length);
        THROW_IF_ALLOC_FAIL($$);
    }
;

block: 
    '{' opt_variable_defs opt_statements '}' {
        DEBUGF("block r1");
    }
;

statement: 
    MON_TK_IF cond block opt_else {
        DEBUGF("statement r1");
    }

    | MON_TK_WHILE cond block {
        DEBUGF("block r2");
    }

    | lvalue '=' exp ';' {
        DEBUGF("block r3");
    }

    | MON_TK_RETURN opt_exp ';' {
        DEBUGF("block r4");
    }

    | call ';' {
        DEBUGF("block r5");
    }

    | '@' exp ';' {
        DEBUGF("block r6");
    }

    | block {
        DEBUGF("block r7");
    }
;

opt_else: 
    /* nothing */ {
        DEBUGF("opt_else r1");
    }

    | MON_TK_ELSE block {
        DEBUGF("opt_else r2");
    }
;

statements: 
    statement {
        DEBUGF("statements r1");
    }
    
    | statements statement {
        DEBUGF("statements r2");
    }
;

opt_statements: 
    /* nothing */  {
        DEBUGF("opt_statements r1");
    }
    | statements {
        DEBUGF("opt_statements r2");
    }
;

var: 
    MON_TK_IDENTIFIER {
        DEBUGF("var r1");
    }
    | var '.' MON_TK_IDENTIFIER {
        DEBUGF("var r2");
    }
;

numeral: 
    MON_TK_LIT_FLOAT {
        DEBUGF("numeral r1");
    }

    | MON_TK_LIT_INT {
        DEBUGF("numeral r2");
    }
;

opt_exp: 
    /* nothing */ {
        DEBUGF("opt_exp r1");
    }

    | exp {
        DEBUGF("opt_exp r2");
    }
;

exp: 
    exp_conditional {
        DEBUGF("exp r1");
    }
;

lvalue: 
    var {
        DEBUGF("lvalue r1");
    }

    | exp_primary bracket_exp {
        DEBUGF("lvalue r2");
    }
;

exp_primary: 
    '(' exp ')' {
        DEBUGF("exp_primary r1");
    }

    | numeral {
        DEBUGF("exp_primary r2");
    }

    | lvalue {
        DEBUGF("exp_primary r3");
    }
;

exp_postfix: 
    exp_primary {
        DEBUGF("exp_postfix r1");
    }

    | exp_primary MON_TK_AS type {
        DEBUGF("exp_postfix r2");
    }
;
              
exp_unary: 
    exp_postfix {
        DEBUGF("exp_unary r1");
    }

    | '-' exp_primary {
        DEBUGF("exp_unary r2");
    }
;

exp_multiplicative: 
    exp_unary {
        DEBUGF("exp_multiplicative r1");
    }

    | exp_multiplicative '*' exp_unary {
        DEBUGF("exp_multiplicative r2");
    }

    | exp_multiplicative '/' exp_unary {
        DEBUGF("exp_multiplicative r3");
    }
;

exp_additive: 
    exp_multiplicative {
        DEBUGF("exp_additive r1");
    }

    | exp_additive '+' exp_multiplicative {
        DEBUGF("exp_additive r2");
    }

    | exp_additive '-' exp_multiplicative {
        DEBUGF("exp_additive r3");
    }
;

exp_conditional: 
    exp_additive {
        DEBUGF("exp_conditional r1");
    }

    | cond_primary '?' exp ':' exp_conditional {
        DEBUGF("exp_conditional r2");
    }

    | MON_TK_NEW type opt_bracket_exp {
        DEBUGF("exp_conditional r3");
    }
;

opt_bracket_exp: 
    /* nothing */ {
        DEBUGF("opt_bracket_exp r1");
    }

    | bracket_exp {
        DEBUGF("opt_bracket_exp r2");
    }
;

bracket_exp: 
    '[' exp ']' {
        DEBUGF("bracket_exp r1");
    }
;

cond: 
    cond_or {
        DEBUGF("cond_or r1");
    }
;

cond_primary: 
    '(' cond ')' {
        DEBUGF("cond_primary r1");
    }

    | exp_additive MON_TK_OP_EQ exp_additive {
        DEBUGF("cond_primary r2");
    }

    | exp_additive MON_TK_OP_NE exp_additive {
        DEBUGF("cond_primary r3");
    }

    | exp_additive MON_TK_OP_LE exp_additive {
        DEBUGF("cond_primary r4");
    }

    | exp_additive MON_TK_OP_GE exp_additive {
        DEBUGF("cond_primary r5");
    }

    | exp_additive '>' exp_additive {
        DEBUGF("cond_primary r6");
    }

    | exp_additive '<' exp_additive {
        DEBUGF("cond_primary r7");
    }
;

cond_not: 
    cond_primary {
        DEBUGF("cond_not r1");
    }

    | '!' '(' cond ')' {
        DEBUGF("cond_not r2");
    }
;

cond_and: 
    cond_not {
        DEBUGF("cond_and r1");
    }

    | cond_and MON_TK_OP_AND cond_not {
        DEBUGF("cond_and r2");
    }
;

cond_or: 
    cond_and {
        DEBUGF("cond_or r1");
    }

    | cond_or MON_TK_OP_OR cond_and {
        DEBUGF("cond_or r2");
    }
;

call: 
    MON_TK_IDENTIFIER '(' opt_exps ')'  {
        DEBUGF("call r1");
    }
;

exps: 
    exp  {
        DEBUGF("exps r1");
    }

    | exps ',' exp  {
        DEBUGF("exps r2");
    }
;

opt_exps: 
    /* nothing */  {
        DEBUGF("opt_exps r1");
    }

    | exps  {
        DEBUGF("opt_exps r2");
    }
;

%%

void yyerror(const char* s) {
    fprintf(stderr, "%s\n", s);
}