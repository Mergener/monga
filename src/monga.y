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
            unsigned length;
    } identifier;
    
    long integer;
    double real;
}

%{
#include <monga.tab.h>
%}

%%

program : definitions 
        | /* nothing */
        ;

definitions : definition
            | definitions definition
            ;

definition : def_variable | def_function | def_type ;

def_variable : MON_TK_VAR MON_TK_IDENTIFIER ':' type ';' ;

variable_defs : def_variable
              | variable_defs def_variable
              ;

type : MON_TK_IDENTIFIER ;

def_type : MON_TK_TYPE MON_TK_IDENTIFIER '=' typedesc ;

typedesc : MON_TK_IDENTIFIER 
         | '[' typedesc ']' 
         | '{' fields '}'
         ;

field : MON_TK_IDENTIFIER ':' type ';' ;

fields : field
       | fields field
       ;

def_function : MON_TK_FUNCTION MON_TK_IDENTIFIER '(' opt_parameters ')' opt_ret_type block ;

opt_ret_type : /* nothing */
             | ':' type
             ;

parameters : parameter
           | parameters ',' parameter
           ;

opt_parameters : /* nothing */ | parameters ;

parameter : MON_TK_IDENTIFIER ':' type ;

block : '{' variable_defs opt_statements '}' ;

statement : MON_TK_IF cond block opt_else
          | MON_TK_WHILE cond block
          | var '=' exp ';'
          | MON_TK_RETURN [ exp ] ';'
          | call ';'
          | '@' exp ';'
          | block 
	  ;

opt_else : /* nothing */
         | MON_TK_ELSE block
         ;

statements : statement
           | statements statement
           ;

opt_statements : /* nothing */ 
               | statements 
               ;

var : MON_TK_IDENTIFIER 
    | var '[' exp ']' 
    | var '.' MON_TK_IDENTIFIER 
    ;

numeral : MON_TK_LIT_FLOAT
        | MON_TK_LIT_INT
        ;

exp : exp_conditional
    ;

exp_primary : '(' exp ')'
            | numeral
            | var
            ;

exp_postfix : exp_primary
            | exp_primary MON_TK_AS type
            ;
              
exp_unary : exp_postfix
          | '-' exp_primary
          ;

exp_multiplicative : exp_unary
                   | exp_multiplicative '*' exp_unary
                   | exp_multiplicative '/' exp_unary
                   ;

exp_additive : exp_multiplicative
             | exp_additive '+' exp_multiplicative
             | exp_additive '-' exp_multiplicative
             ;

exp_conditional : exp_additive
                | cond_primary '?' exp ':' exp_conditional
                | MON_TK_NEW type opt_bracket_exp
                ;

opt_bracket_exp : /* nothing */
                | '[' exp ']'
                ;

cond : cond_and
     ;

cond_primary : '(' cond ')'
             | exp_additive MON_TK_OP_EQ exp_additive
             | exp_additive MON_TK_OP_NE exp_additive
             | exp_additive MON_TK_OP_LE exp_additive
             | exp_additive MON_TK_OP_GE exp_additive
             | exp_additive '>' exp_additive
             | exp_additive '<' exp_additive
             ;

cond_not : cond_primary
         | '!' '(' cond ')'
         ;

cond_and : cond_not
         | cond_and MON_TK_OP_AND cond_not
         ;

cond_or : cond_and
        | cond_or MON_TK_OP_OR cond_and
        ;

call : MON_TK_IDENTIFIER '(' opt_exps ')' ;

exps : exp
     | exps ',' exp
     ;

opt_exps : /* nothing */
         | exps
         ;

%%