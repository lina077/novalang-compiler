#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

typedef struct Symbol {
    char *name;
    Value val;
    ASTNode *func_body;
    char **params;
    int param_count;
    int is_function;
    struct Symbol *next;
} Symbol;

typedef struct Scope {
    Symbol *symbols;
    struct Scope *parent;
} Scope;

void push_scope();
void pop_scope();

int add_variable(const char *name, Value val);
int set_variable(const char *name, Value val);
Value get_variable(const char *name, int *err);

int add_function(const char *name, ASTNode *body, char **params, int param_count);
Symbol* get_function(const char *name);

void free_symbol_table();

#endif
