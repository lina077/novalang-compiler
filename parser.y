%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "error.h"

int yylex(void);
void yyerror(const char *s);

ASTNode *root_ast = NULL;
%}

%union {
    double fval;
    char *sval;
    ASTNode *ast;
}

%token <fval> NUMBER
%token <sval> IDENTIFIER STRING
%token VAR IF ELSE WHILE FN RETURN PRINT INPUT
%token PLUS MINUS MUL DIV ASSIGN
%token GT LT GE LE EQ NE AND OR NOT
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA

%type <ast> program statement_list statement declaration assignment print_stmt input_stmt if_stmt while_stmt function_def block expr opt_arg_list arg_list opt_param_list param_list return_stmt

%left OR
%left AND
%left EQ NE
%left GT LT GE LE
%left PLUS MINUS
%left MUL DIV
%right NOT
%right ASSIGN

%%

program:
    statement_list { root_ast = $1; }
    ;

statement_list:
    statement_list statement {
        add_child_node($1, $2);
        $$ = $1;
    }
    | statement {
        $$ = create_block_node();
        add_child_node($$, $1);
    }
    ;

statement:
      declaration
    | assignment
    | print_stmt
    | input_stmt
    | if_stmt
    | while_stmt
    | function_def
    | return_stmt
    | expr SEMICOLON { $$ = $1; }
    ;

declaration:
    VAR IDENTIFIER ASSIGN expr SEMICOLON {
        $$ = create_decl_node($2, $4);
        free($2);
    }
    | VAR IDENTIFIER SEMICOLON {
        $$ = create_decl_node($2, create_num_node(0));
        free($2);
    }
    ;

assignment:
    IDENTIFIER ASSIGN expr SEMICOLON {
        $$ = create_assign_node($1, $3);
        free($1);
    }
    ;

print_stmt:
    PRINT LPAREN expr RPAREN SEMICOLON {
        $$ = create_print_node($3);
    }
    ;

input_stmt:
    IDENTIFIER ASSIGN INPUT LPAREN RPAREN SEMICOLON {
        $$ = create_input_node($1);
        free($1);
    }
    ;

if_stmt:
    IF LPAREN expr RPAREN block {
        $$ = create_if_node($3, $5, NULL);
    }
    | IF LPAREN expr RPAREN block ELSE block {
        $$ = create_if_node($3, $5, $7);
    }
    ;

while_stmt:
    WHILE LPAREN expr RPAREN block {
        $$ = create_while_node($3, $5);
    }
    ;

function_def:
    FN IDENTIFIER LPAREN opt_param_list RPAREN block {
        ASTNode *fn = create_func_def_node($2, $6);
        if ($4) {
            for (int i = 0; i < $4->param_count; i++) {
                add_param_to_func(fn, $4->params[i]);
            }
            free_ast($4);
        }
        free($2);
        $$ = fn;
    }
    ;

opt_param_list:
    param_list { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

param_list:
    param_list COMMA IDENTIFIER {
        add_param_to_func($1, $3);
        free($3);
        $$ = $1;
    }
    | IDENTIFIER {
        $$ = create_block_node();
        add_param_to_func($$, $1);
        free($1);
    }
    ;

return_stmt:
    RETURN expr SEMICOLON { $$ = create_return_node($2); }
    | RETURN SEMICOLON { $$ = create_return_node(NULL); }
    ;

block:
    LBRACE statement_list RBRACE { $$ = $2; }
    | LBRACE RBRACE { $$ = create_block_node(); }
    ;

expr:
      expr PLUS expr       { $$ = create_binop_node(PLUS, $1, $3); }
    | expr MINUS expr      { $$ = create_binop_node(MINUS, $1, $3); }
    | expr MUL expr        { $$ = create_binop_node(MUL, $1, $3); }
    | expr DIV expr        { $$ = create_binop_node(DIV, $1, $3); }
    | expr GT expr         { $$ = create_binop_node(GT, $1, $3); }
    | expr LT expr         { $$ = create_binop_node(LT, $1, $3); }
    | expr GE expr         { $$ = create_binop_node(GE, $1, $3); }
    | expr LE expr         { $$ = create_binop_node(LE, $1, $3); }
    | expr EQ expr         { $$ = create_binop_node(EQ, $1, $3); }
    | expr NE expr         { $$ = create_binop_node(NE, $1, $3); }
    | expr AND expr        { $$ = create_binop_node(AND, $1, $3); }
    | expr OR expr         { $$ = create_binop_node(OR, $1, $3); }
    | NOT expr             { $$ = create_unop_node(NOT, $2); }
    | MINUS expr %prec NOT { $$ = create_unop_node(MINUS, $2); }
    | LPAREN expr RPAREN   { $$ = $2; }
    | NUMBER               { $$ = create_num_node($1); }
    | STRING               { $$ = create_str_node($1); free($1); }
    | IDENTIFIER           { $$ = create_var_node($1); free($1); }
    | IDENTIFIER LPAREN opt_arg_list RPAREN {
        ASTNode *call = create_func_call_node($1);
        if ($3) {
            for (int i = 0; i < $3->child_count; i++) {
                add_arg_to_call(call, $3->children[i]);
            }
            free($3->children);
            free($3);
        }
        free($1);
        $$ = call;
    }
    ;

opt_arg_list:
    arg_list { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

arg_list:
    arg_list COMMA expr {
        add_child_node($1, $3);
        $$ = $1;
    }
    | expr {
        $$ = create_block_node();
        add_child_node($$, $1);
    }
    ;

%%

void yyerror(const char *s) {
    print_error(s);
}
