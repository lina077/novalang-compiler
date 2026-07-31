#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static ASTNode* alloc_node(NodeType type) {
    ASTNode *node = (ASTNode*)calloc(1, sizeof(ASTNode));
    node->type = type;
    return node;
}

ASTNode* create_num_node(double val) {
    ASTNode *node = alloc_node(NODE_NUM);
    node->val.type = VAL_NUMBER;
    node->val.number = val;
    return node;
}

ASTNode* create_str_node(const char *val) {
    ASTNode *node = alloc_node(NODE_STR);
    node->val.type = VAL_STRING;
    node->val.string = strdup(val);
    return node;
}

ASTNode* create_var_node(const char *name) {
    ASTNode *node = alloc_node(NODE_VAR);
    node->name = strdup(name);
    return node;
}

ASTNode* create_decl_node(const char *name, ASTNode *expr) {
    ASTNode *node = alloc_node(NODE_DECL);
    node->name = strdup(name);
    node->left = expr;
    return node;
}

ASTNode* create_assign_node(const char *name, ASTNode *expr) {
    ASTNode *node = alloc_node(NODE_ASSIGN);
    node->name = strdup(name);
    node->left = expr;
    return node;
}

ASTNode* create_binop_node(int op, ASTNode *left, ASTNode *right) {
    ASTNode *node = alloc_node(NODE_BINOP);
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* create_unop_node(int op, ASTNode *operand) {
    ASTNode *node = alloc_node(NODE_UNOP);
    node->op = op;
    node->left = operand;
    return node;
}

ASTNode* create_print_node(ASTNode *expr) {
    ASTNode *node = alloc_node(NODE_PRINT);
    node->left = expr;
    return node;
}

ASTNode* create_input_node(const char *var_name) {
    ASTNode *node = alloc_node(NODE_INPUT);
    node->name = strdup(var_name);
    return node;
}

ASTNode* create_if_node(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *node = alloc_node(NODE_IF);
    node->left = cond;
    node->right = then_branch;
    node->third = else_branch;
    return node;
}

ASTNode* create_while_node(ASTNode *cond, ASTNode *body) {
    ASTNode *node = alloc_node(NODE_WHILE);
    node->left = cond;
    node->right = body;
    return node;
}

ASTNode* create_block_node() {
    return alloc_node(NODE_BLOCK);
}

void add_child_node(ASTNode *block, ASTNode *child) {
    if (!block || !child) return;
    block->child_count++;
    block->children = realloc(block->children, sizeof(ASTNode*) * block->child_count);
    block->children[block->child_count - 1] = child;
}

ASTNode* create_func_def_node(const char *name, ASTNode *body) {
    ASTNode *node = alloc_node(NODE_FUNC_DEF);
    node->name = strdup(name);
    node->right = body;
    return node;
}

void add_param_to_func(ASTNode *func, const char *param_name) {
    func->param_count++;
    func->params = realloc(func->params, sizeof(char*) * func->param_count);
    func->params[func->param_count - 1] = strdup(param_name);
}

ASTNode* create_func_call_node(const char *name) {
    ASTNode *node = alloc_node(NODE_FUNC_CALL);
    node->name = strdup(name);
    return node;
}

void add_arg_to_call(ASTNode *call, ASTNode *arg) {
    call->child_count++;
    call->children = realloc(call->children, sizeof(ASTNode*) * call->child_count);
    call->children[call->child_count - 1] = arg;
}

ASTNode* create_return_node(ASTNode *expr) {
    ASTNode *node = alloc_node(NODE_RETURN);
    node->left = expr;
    return node;
}

void free_value(Value v) {
    if (v.type == VAL_STRING && v.string) {
        free(v.string);
    }
}

void free_ast(ASTNode *node) {
    if (!node) return;
    if (node->name) free(node->name);
    if (node->type == NODE_STR) free_value(node->val);
    
    if (node->left) free_ast(node->left);
    if (node->right) free_ast(node->right);
    if (node->third) free_ast(node->third);
    
    for (int i = 0; i < node->child_count; i++) {
        free_ast(node->children[i]);
    }
    if (node->children) free(node->children);
    
    for (int i = 0; i < node->param_count; i++) {
        free(node->params[i]);
    }
    if (node->params) free(node->params);
    
    free(node);
}
