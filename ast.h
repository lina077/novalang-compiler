#ifndef AST_H
#define AST_H

typedef enum {
    VAL_NUMBER,
    VAL_STRING,
    VAL_VOID
} ValueType;

typedef struct {
    ValueType type;
    union {
        double number;
        char *string;
    };
} Value;

typedef enum {
    NODE_NUM,
    NODE_STR,
    NODE_VAR,
    NODE_DECL,
    NODE_ASSIGN,
    NODE_BINOP,
    NODE_UNOP,
    NODE_PRINT,
    NODE_INPUT,
    NODE_IF,
    NODE_WHILE,
    NODE_BLOCK,
    NODE_FUNC_DEF,
    NODE_FUNC_CALL,
    NODE_RETURN
} NodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
    NodeType type;
    Value val;
    char *name;
    int op;
    
    ASTNode *left;
    ASTNode *right;
    ASTNode *third; // used for else-block or extra params
    
    ASTNode **children;
    int child_count;
    
    char **params;
    int param_count;
};

ASTNode* create_num_node(double val);
ASTNode* create_str_node(const char *val);
ASTNode* create_var_node(const char *name);
ASTNode* create_decl_node(const char *name, ASTNode *expr);
ASTNode* create_assign_node(const char *name, ASTNode *expr);
ASTNode* create_binop_node(int op, ASTNode *left, ASTNode *right);
ASTNode* create_unop_node(int op, ASTNode *operand);
ASTNode* create_print_node(ASTNode *expr);
ASTNode* create_input_node(const char *var_name);
ASTNode* create_if_node(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch);
ASTNode* create_while_node(ASTNode *cond, ASTNode *body);
ASTNode* create_block_node();
void add_child_node(ASTNode *block, ASTNode *child);
ASTNode* create_func_def_node(const char *name, ASTNode *body);
void add_param_to_func(ASTNode *func, const char *param_name);
ASTNode* create_func_call_node(const char *name);
void add_arg_to_call(ASTNode *call, ASTNode *arg);
ASTNode* create_return_node(ASTNode *expr);

void free_ast(ASTNode *node);
void free_value(Value v);

#endif
