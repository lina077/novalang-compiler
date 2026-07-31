#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

static Scope *current_scope = NULL;

void push_scope() {
    Scope *new_scope = (Scope*)calloc(1, sizeof(Scope));
    new_scope->parent = current_scope;
    current_scope = new_scope;
}

void pop_scope() {
    if (!current_scope) return;
    Scope *parent = current_scope->parent;
    Symbol *curr = current_scope->symbols;
    while (curr) {
        Symbol *tmp = curr;
        curr = curr->next;
        free(tmp->name);
        free_value(tmp->val);
        free(tmp);
    }
    free(current_scope);
    current_scope = parent;
}

int add_variable(const char *name, Value val) {
    if (!current_scope) push_scope();
    
    Symbol *curr = current_scope->symbols;
    while (curr) {
        if (strcmp(curr->name, name) == 0) return 0; // Exists in current scope
        curr = curr->next;
    }
    
    Symbol *sym = (Symbol*)calloc(1, sizeof(Symbol));
    sym->name = strdup(name);
    sym->val = val;
    sym->is_function = 0;
    sym->next = current_scope->symbols;
    current_scope->symbols = sym;
    return 1;
}

int set_variable(const char *name, Value val) {
    Scope *sc = current_scope;
    while (sc) {
        Symbol *curr = sc->symbols;
        while (curr) {
            if (!curr->is_function && strcmp(curr->name, name) == 0) {
                free_value(curr->val);
                curr->val = val;
                return 1;
            }
            curr = curr->next;
        }
        sc = sc->parent;
    }
    return add_variable(name, val);
}

Value get_variable(const char *name, int *err) {
    Scope *sc = current_scope;
    *err = 0;
    while (sc) {
        Symbol *curr = sc->symbols;
        while (curr) {
            if (!curr->is_function && strcmp(curr->name, name) == 0) {
                return curr->val;
            }
            curr = curr->next;
        }
        sc = sc->parent;
    }
    *err = 1;
    Value v = {VAL_VOID, {0}};
    return v;
}

int add_function(const char *name, ASTNode *body, char **params, int param_count) {
    if (!current_scope) push_scope();
    
    Symbol *sym = (Symbol*)calloc(1, sizeof(Symbol));
    sym->name = strdup(name);
    sym->func_body = body;
    sym->params = params;
    sym->param_count = param_count;
    sym->is_function = 1;
    sym->next = current_scope->symbols;
    current_scope->symbols = sym;
    return 1;
}

Symbol* get_function(const char *name) {
    Scope *sc = current_scope;
    while (sc) {
        Symbol *curr = sc->symbols;
        while (curr) {
            if (curr->is_function && strcmp(curr->name, name) == 0) {
                return curr;
            }
            curr = curr->next;
        }
        sc = sc->parent;
    }
    return NULL;
}

void free_symbol_table() {
    while (current_scope) {
        pop_scope();
    }
}
