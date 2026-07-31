#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "eval.h"
#include "symbol_table.h"
#include "error.h"
#include "parser.tab.h"

static int return_flag = 0;
static Value return_val;

Value eval_ast(ASTNode *node) {
    Value result = {VAL_VOID, {0}};
    if (!node || return_flag) return result;

    switch (node->type) {
        case NODE_NUM:
            return node->val;

        case NODE_STR: {
            result.type = VAL_STRING;
            result.string = strdup(node->val.string);
            return result;
        }

        case NODE_VAR: {
            int err = 0;
            Value v = get_variable(node->name, &err);
            if (err) {
                print_error_var("Undefined variable", node->name);
                exit(1);
            }
            if (v.type == VAL_STRING) {
                result.type = VAL_STRING;
                result.string = strdup(v.string);
                return result;
            }
            return v;
        }

        case NODE_DECL: {
            Value val = eval_ast(node->left);
            if (!add_variable(node->name, val)) {
                print_error_var("Variable already declared", node->name);
            }
            break;
        }

        case NODE_ASSIGN: {
            Value val = eval_ast(node->left);
            set_variable(node->name, val);
            break;
        }

        case NODE_BINOP: {
            Value l = eval_ast(node->left);
            Value r = eval_ast(node->right);

            if (l.type == VAL_STRING || r.type == VAL_STRING) {
                if (node->op == PLUS) {
                    char buf1[64], buf2[64];
                    const char *s1 = (l.type == VAL_STRING) ? l.string : (snprintf(buf1, sizeof(buf1), "%g", l.number), buf1);
                    const char *s2 = (r.type == VAL_STRING) ? r.string : (snprintf(buf2, sizeof(buf2), "%g", r.number), buf2);
                    
                    char *res = malloc(strlen(s1) + strlen(s2) + 1);
                    strcpy(res, s1);
                    strcat(res, s2);
                    
                    free_value(l); free_value(r);
                    result.type = VAL_STRING;
                    result.string = res;
                    return result;
                } else {
                    print_error("Invalid string operation");
                    exit(1);
                }
            }

            result.type = VAL_NUMBER;
            switch (node->op) {
                case PLUS:  result.number = l.number + r.number; break;
                case MINUS: result.number = l.number - r.number; break;
                case MUL:   result.number = l.number * r.number; break;
                case DIV:
                    if (r.number == 0) {
                        print_error("Division by zero");
                        exit(1);
                    }
                    result.number = l.number / r.number;
                    break;
                case GT:  result.number = l.number > r.number; break;
                case LT:  result.number = l.number < r.number; break;
                case GE:  result.number = l.number >= r.number; break;
                case LE:  result.number = l.number <= r.number; break;
                case EQ:  result.number = l.number == r.number; break;
                case NE:  result.number = l.number != r.number; break;
                case AND: result.number = l.number && r.number; break;
                case OR:  result.number = l.number || r.number; break;
            }
            return result;
        }

        case NODE_UNOP: {
            Value operand = eval_ast(node->left);
            result.type = VAL_NUMBER;
            if (node->op == NOT) result.number = !operand.number;
            else if (node->op == MINUS) result.number = -operand.number;
            return result;
        }

        case NODE_PRINT: {
            Value v = eval_ast(node->left);
            if (v.type == VAL_NUMBER) printf("%g\n", v.number);
            else if (v.type == VAL_STRING) printf("%s\n", v.string);
            free_value(v);
            break;
        }

        case NODE_INPUT: {
            double val;
            printf("Input for %s: ", node->name);
            if (scanf("%lf", &val) == 1) {
                Value v = {VAL_NUMBER, {.number = val}};
                set_variable(node->name, v);
            } else {
                print_error("Invalid input format");
                exit(1);
            }
            break;
        }

        case NODE_IF: {
            Value cond = eval_ast(node->left);
            if (cond.number) {
                eval_ast(node->right);
            } else if (node->third) {
                eval_ast(node->third);
            }
            break;
        }

        case NODE_WHILE: {
            while (1) {
                Value cond = eval_ast(node->left);
                if (!cond.number || return_flag) break;
                eval_ast(node->right);
            }
            break;
        }

        case NODE_BLOCK: {
            push_scope();
            for (int i = 0; i < node->child_count && !return_flag; i++) {
                eval_ast(node->children[i]);
            }
            pop_scope();
            break;
        }

        case NODE_FUNC_DEF: {
            add_function(node->name, node->right, node->params, node->param_count);
            break;
        }

        case NODE_FUNC_CALL: {
            Symbol *fn = get_function(node->name);
            if (!fn) {
                print_error_var("Undefined function", node->name);
                exit(1);
            }
            
            Value *args = malloc(sizeof(Value) * node->child_count);
            for (int i = 0; i < node->child_count; i++) {
                args[i] = eval_ast(node->children[i]);
            }

            push_scope();
            for (int i = 0; i < fn->param_count && i < node->child_count; i++) {
                add_variable(fn->params[i], args[i]);
            }

            int prev_return_flag = return_flag;
            return_flag = 0;

            eval_ast(fn->func_body);

            Value fn_res = return_val;
            return_flag = prev_return_flag;

            pop_scope();
            free(args);
            return fn_res;
        }

        case NODE_RETURN: {
            if (node->left) {
                return_val = eval_ast(node->left);
            } else {
                return_val.type = VAL_VOID;
            }
            return_flag = 1;
            break;
        }
    }
    return result;
}
