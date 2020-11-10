// The following is needed since the YACC generated header file must
// be aware of types defined in these headers.
%code requires {
#include "mon_ast.h"
#include "mon_vector.h"
#include "mon_literal.h"
}

%{

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "grammar.tab.h"
#include "mon_alloc.h"
#include "sem/types.h"
#include "strutils.h"

#define THROW_IF_ALLOC_FAIL(var)
#define INIT_VECTOR(vec) \
    if (Mon_VectorInit(&vec) != MON_SUCCESS) { \
    }
#define ADD_TO_VECTOR(vec, value) \
    if (Mon_VectorPush(&vec, value) != MON_SUCCESS) { \
    }
#define FILL_NODE_HEADER(header) \
    header.line = Mon_TkLine; \
    header.column = Mon_TkColumn

int yylex();
void yyerror(const char* s);

/** 
 *  Pointer to the user provided AST object to be filled.  
 *
 *  See parser.c for detailed information. 
 */
extern Mon_Ast* mon_TargetAst;

extern int Mon_TkLine;
extern int Mon_TkColumn;

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
%token MON_TK_BREAK
%token MON_TK_CONTINUE
%token MON_TK_NEW
%token MON_TK_AS
%token MON_TK_NULL
%token MON_TK_LEN
%token MON_TK_LIT_INT
%token MON_TK_LIT_FLOAT
%token MON_TK_LIT_STRING
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
    Mon_Literal literal;

    struct {
        char* name;
        size_t length;
    } identifier;

    Mon_Ast* ast;

    Mon_Vector        vector;
    bool              boolean;
    Mon_AstDef*       defNode;
    Mon_AstBlock*     blockNode;
    Mon_AstTypeDef*   typeDefNode;
    Mon_AstVarDef*    varDefNode;
    Mon_AstFuncDef*   funcDefNode;
    Mon_AstStatement* statementNode;
    Mon_AstExp*       expNode;
    Mon_AstCond*      condNode;
    Mon_AstParam*     paramNode;
    Mon_AstVar*       varNode;
    Mon_AstCall*      callNode;
    Mon_AstTypeDesc*  typeDescNode;
    Mon_AstField*     fieldNode;
    
}

%type <identifier>    MON_TK_IDENTIFIER type opt_ret_type
%type <blockNode>     block opt_else
%type <defNode>       definition   
%type <varDefNode>    def_variable
%type <typeDefNode>   def_type
%type <funcDefNode>   def_function
%type <paramNode>     parameter
%type <statementNode> statement
%type <varNode>       var
%type <expNode>       opt_exp exp exp_primary exp_postfix exp_unary exp_multiplicative exp_additive exp_conditional opt_bracket_exp bracket_exp
%type <condNode>      cond cond_primary cond_not cond_and cond_or
%type <callNode>      call
%type <literal>       numeral MON_TK_LIT_FLOAT MON_TK_LIT_INT MON_TK_LIT_STRING
%type <vector>        definitions parameters opt_parameters statements opt_statements exps opt_exps field_defs
%type <typeDescNode>  typedesc
%type <fieldNode>     def_field

%%

module: 
    definitions {
        DumpReduce("module r1");

        mon_TargetAst->defsVector = $1;
    }

    | /* nothing */ {
        DumpReduce("module r2");

        INIT_VECTOR(mon_TargetAst->defsVector);
    }
;

definitions: 
    definition {
        DumpReduce("definitions r1");

        INIT_VECTOR($$);
        ADD_TO_VECTOR($$, $1);
    }

    | definitions definition {
        DumpReduce("definitions r2");

        $$ = $1;
        ADD_TO_VECTOR($$, $2);
    }
;

definition: 
    def_variable { 
        DumpReduce("definition r1");

        $$ = Mon_AstDefNewVar($1); 

        THROW_IF_ALLOC_FAIL($$);

        FILL_NODE_HEADER($$->header);
    }

    | def_function { 
        DumpReduce("definition r2");

        $$ = Mon_AstDefNewFunc($1); 

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | def_type { 
        DumpReduce("definition r3");

        $$ = Mon_AstDefNewType($1); 

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

def_variable: 
    MON_TK_VAR MON_TK_IDENTIFIER ':' type ';' {
        DumpReduce("def_variable r1");

        $$ = Mon_AstVarDefNew($2.name, $2.length, $4.name, $4.length);

        Mon_Free($2.name);
        Mon_Free($4.name);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

type: 
    MON_TK_IDENTIFIER {
        DumpReduce("type r1");

        $$ = $1;
    }
;

def_type: 
    MON_TK_TYPE MON_TK_IDENTIFIER '=' typedesc ';' {
        DumpReduce("def_type r1");

        $$ = Mon_AstTypeDefNew($2.name, $2.length, $4);
        
        Mon_Free($2.name);
        
        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

typedesc: 
    MON_TK_IDENTIFIER {
        DumpReduce("typedesc r1");

        $$ = Mon_AstTypeDescNewAlias($1.name);
        
        Mon_Free($1.name);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | '[' typedesc ']' {
        DumpReduce("typedesc r2");

        $$ = Mon_AstTypeDescNewArray($2);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | '{' field_defs '}' {
        DumpReduce("typedesc r3");

        $$ = Mon_AstTypeDescNewRecord(&$2);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

def_field: 
    MON_TK_IDENTIFIER ':' type ';' {
        DumpReduce("def_field r1");

        $$ = Mon_AstFieldNew($3.name, $1.name);
        
        Mon_Free($1.name);        
        Mon_Free($3.name);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

field_defs: 
    def_field {
        DumpReduce("field_defs r1");

        INIT_VECTOR($$);
        ADD_TO_VECTOR($$, $1);
    }

    | field_defs def_field {
        DumpReduce("field_defs r2");

        $$ = $1;
        ADD_TO_VECTOR($$, $2);
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
        
        Mon_Free($2.name);        
        Mon_Free($6.name);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

opt_ret_type: 
    /* nothing */ {
        DumpReduce("opt_ret_type r1");

        int len = strlen(TYPENAME_VOID);
        $$.name = DuplicateString(TYPENAME_VOID, len);
        $$.length = len;
    }

    | ':' type {
        DumpReduce("opt_ret_type r2");

        $$ = $2;
    }
;

parameters: 
    parameter {
        DumpReduce("parameters r1");        
        
        INIT_VECTOR($$);
        ADD_TO_VECTOR($$, $1);
    }

    | parameters ',' parameter {
        DumpReduce("parameters r2");

        $$ = $1;
        ADD_TO_VECTOR($$, $3);
    }
;

opt_parameters: 
    /* nothing */ {
        DumpReduce("opt_parameters r1");

        INIT_VECTOR($$);
    }

    | parameters {
        DumpReduce("opt_parameters r2");

        $$ = $1;
    }
;

parameter: 
    MON_TK_IDENTIFIER ':' type {
        DumpReduce("parameter r1");

        $$ = Mon_AstParamNew($1.name, $1.length, $3.name, $3.length);

        Mon_Free($1.name);
        Mon_Free($3.name);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

block: 
    '{' opt_statements '}' {
        DumpReduce("block r1");

        $$ = Mon_AstBlockNew($2);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

statement: 
    MON_TK_IF cond block opt_else {
        DumpReduce("statement r1");

        $$ = Mon_AstStatementNewIf($2, $3, $4);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | MON_TK_WHILE cond block {
        DumpReduce("block r2");

        $$ = Mon_AstStatementNewWhile($2, $3);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | var '=' exp ';' {
        DumpReduce("block r3");

        $$ = Mon_AstStatementNewAssignment($1, $3);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | MON_TK_RETURN opt_exp ';' {
        DumpReduce("block r4");

        $$ = Mon_AstStatementNewReturn($2);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | call ';' {
        DumpReduce("block r5");

        $$ = Mon_AstStatementNewCall($1);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | '@' exp ';' {
        DumpReduce("block r6");

        $$ = Mon_AstStatementNewEcho($2);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | block {
        DumpReduce("block r7");

        $$ = Mon_AstStatementNewBlock($1);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | def_variable {
        DumpReduce("block r8");

        $$ = Mon_AstStatementNewVarDef($1);

        THROW_IF_ALLOC_FAIL($$);

        FILL_NODE_HEADER($$->header);
    }

    | MON_TK_BREAK ';' {
        DumpReduce("block r9");

        $$ = Mon_AstStatementNewBreak();

        THROW_IF_ALLOC_FAIL($$);

        FILL_NODE_HEADER($$->header);
    }

    | MON_TK_CONTINUE ';' {
        DumpReduce("block r10");

        $$ = Mon_AstStatementNewContinue();

        THROW_IF_ALLOC_FAIL($$);

        FILL_NODE_HEADER($$->header);
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

        INIT_VECTOR($$);
        ADD_TO_VECTOR($$, $1);
    }

    | statements statement {
        DumpReduce("statements r2");

        $$ = $1;
        ADD_TO_VECTOR($$, $2);
    }
;

opt_statements: 
    /* nothing */  {
        DumpReduce("opt_statements r1");

        INIT_VECTOR($$);
    }

    | statements {
        DumpReduce("opt_statements r2");

        $$ = $1;
    }
;

var: 
    MON_TK_IDENTIFIER {
        DumpReduce("var r1");

        $$ = Mon_AstVarNewDirect($1.name);

        Mon_Free($1.name);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | exp_primary '.' MON_TK_IDENTIFIER {
        DumpReduce("var r2");

        $$ = Mon_AstVarNewField($1, $3.name);

        Mon_Free($3.name);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | exp_primary bracket_exp {
        DumpReduce("var r3");

        $$ = Mon_AstVarNewIndexed($1, $2);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

numeral: 
    MON_TK_LIT_FLOAT {
        DumpReduce("numeral r1");

        $$ = $1;
    }

    | MON_TK_LIT_INT {
        DumpReduce("numeral r2");

        $$ = $1;
    }
;

opt_exp: 
    /* nothing */ {
        DumpReduce("opt_exp r1");

        $$ = NULL;
    }

    | exp {
        DumpReduce("opt_exp r2");

        $$ = $1;
    }
;

exp: 
    exp_conditional {
        DumpReduce("exp r1");

        $$ = $1;
    }
;

exp_primary: 
    '(' exp ')' {
        DumpReduce("exp_primary r1");

        $$ = $2;
    }

    | numeral {
        DumpReduce("exp_primary r2");

        $$ = Mon_AstExpNewLiteral($1);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | var {
        DumpReduce("exp_primary r3");

        $$ = Mon_AstExpNewVar($1);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | call {
        DumpReduce("exp_primary r4");

        $$ = Mon_AstExpNewCall($1);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | MON_TK_NULL {
        DumpReduce("exp_primary r5");

        $$ = Mon_AstExpNewNull();

        THROW_IF_ALLOC_FAIL($$);

        FILL_NODE_HEADER($$->header);
    }

    | MON_TK_LEN '(' exp ')' {
        DumpReduce("exp_primary r6");

        $$ = Mon_AstExpNewUn($3, MON_UNOP_LEN);

        THROW_IF_ALLOC_FAIL($$);

        FILL_NODE_HEADER($$->header);
    }

    | MON_TK_LIT_STRING {
        DumpReduce("exp_primary r7");

        $$ = Mon_AstExpNewLiteral($1);

        THROW_IF_ALLOC_FAIL($$);

        FILL_NODE_HEADER($$->header);
    }
;

exp_postfix: 
    exp_primary {
        DumpReduce("exp_postfix r1");

        $$ = $1;
    }

    | exp_primary MON_TK_AS type {
        DumpReduce("exp_postfix r2");

        $$ = Mon_AstExpNewCast($1, $3.name);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;
              
exp_unary: 
    exp_postfix {
        DumpReduce("exp_unary r1");

        $$ = $1;
    }

    | '-' exp_primary {
        DumpReduce("exp_unary r2");

        $$ = Mon_AstExpNewUn($2, MON_UNOP_NEGATIVE);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

exp_multiplicative: 
    exp_unary {
        DumpReduce("exp_multiplicative r1");

        $$ = $1;
    }

    | exp_multiplicative '*' exp_unary {
        DumpReduce("exp_multiplicative r2");

        $$ = Mon_AstExpNewBin($1, $3, MON_BINOP_MUL);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | exp_multiplicative '/' exp_unary {
        DumpReduce("exp_multiplicative r3");

        $$ = Mon_AstExpNewBin($1, $3, MON_BINOP_DIV);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

exp_additive: 
    exp_multiplicative {
        DumpReduce("exp_additive r1");

        $$ = $1;
    }

    | exp_additive '+' exp_multiplicative {
        DumpReduce("exp_additive r2");

        $$ = Mon_AstExpNewBin($1, $3, MON_BINOP_ADD);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | exp_additive '-' exp_multiplicative {
        DumpReduce("exp_additive r3");

        $$ = Mon_AstExpNewBin($1, $3, MON_BINOP_SUB);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

exp_conditional: 
    exp_additive {
        DumpReduce("exp_conditional r1");

        $$ = $1;
    }

    | cond_primary '?' exp ':' exp_conditional {
        DumpReduce("exp_conditional r2");

        $$ = Mon_AstExpNewCond($1, $3, $5);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | MON_TK_NEW type opt_bracket_exp {
        DumpReduce("exp_conditional r3");

        $$ = Mon_AstExpNewNew($2.name, $3);

        Mon_Free($2.name);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

opt_bracket_exp: 
    /* nothing */ {
        DumpReduce("opt_bracket_exp r1");

        $$ = NULL;
    }

    | bracket_exp {
        DumpReduce("opt_bracket_exp r2");

        $$ = $1;
    }
;

bracket_exp: 
    '[' exp ']' {
        DumpReduce("bracket_exp r1");

        $$ = $2;
    }
;

cond: 
    cond_or {
        DumpReduce("cond_or r1");

        $$ = $1;
    }
;

cond_primary: 
    '(' cond ')' {
        DumpReduce("cond_primary r1");

        $$ = $2;
    }

    | exp_additive MON_TK_OP_EQ exp_additive {
        DumpReduce("cond_primary r2");

        $$ = Mon_AstCondNewCompar($1, $3, MON_COMPAR_EQ);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | exp_additive MON_TK_OP_NE exp_additive {
        DumpReduce("cond_primary r3");

        $$ = Mon_AstCondNewCompar($1, $3, MON_COMPAR_NE);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | exp_additive MON_TK_OP_LE exp_additive {
        DumpReduce("cond_primary r4");

        $$ = Mon_AstCondNewCompar($1, $3, MON_COMPAR_LE);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | exp_additive MON_TK_OP_GE exp_additive {
        DumpReduce("cond_primary r5");

        $$ = Mon_AstCondNewCompar($1, $3, MON_COMPAR_GE);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | exp_additive '>' exp_additive {
        DumpReduce("cond_primary r6");

        $$ = Mon_AstCondNewCompar($1, $3, MON_COMPAR_GT);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }

    | exp_additive '<' exp_additive {
        DumpReduce("cond_primary r7");

        $$ = Mon_AstCondNewCompar($1, $3, MON_COMPAR_LT);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

cond_not: 
    cond_primary {
        DumpReduce("cond_not r1");

        $$ = $1;
    }

    | '!' '(' cond ')' {
        DumpReduce("cond_not r2");

        $$ = $3;
        $$->negate = !($$->negate);
    }
;

cond_and: 
    cond_not {
        DumpReduce("cond_and r1");

        $$ = $1;
    }

    | cond_and MON_TK_OP_AND cond_not {
        DumpReduce("cond_and r2");

        $$ = Mon_AstCondNewBin($1, $3, MON_BINCOND_AND);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

cond_or: 
    cond_and {
        DumpReduce("cond_or r1");

        $$ = $1;
    }

    | cond_or MON_TK_OP_OR cond_and {
        DumpReduce("cond_or r2");

        $$ = Mon_AstCondNewBin($1, $3, MON_BINCOND_OR);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

call: 
    MON_TK_IDENTIFIER '(' opt_exps ')'  {
        DumpReduce("call r1");

        $$ = Mon_AstCallNew($1.name, $1.length, $3);

        Mon_Free($1.name);

        THROW_IF_ALLOC_FAIL($$);
        
        FILL_NODE_HEADER($$->header);
    }
;

exps: 
    exp {
        DumpReduce("exps r1");

        INIT_VECTOR($$);
        ADD_TO_VECTOR($$, $1);
    }

    | exps ',' exp  {
        DumpReduce("exps r2");

        $$ = $1;
        ADD_TO_VECTOR($$, $3);
    }
;

opt_exps: 
    /* nothing */  {
        DumpReduce("opt_exps r1");

        INIT_VECTOR($$);
    }

    | exps  {
        DumpReduce("opt_exps r2");

        $$ = $1;
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