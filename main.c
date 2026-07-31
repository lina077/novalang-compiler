#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "eval.h"
#include "symbol_table.h"
#include "error.h"

extern int yyparse();
extern FILE *yyin;
extern ASTNode *root_ast;

int main(int argc, char *argv[]) {
    printf(COLOR_CYAN);
    printf("╔══════════════════════════════════════════╗\n");
    printf("║          NovaLang Interpreter v2.0       ║\n");
    if (argc >= 2) {
        printf("║          Running: %-22s ║\n", argv[1]);
    } else {
        printf("║          Running: No file provided      ║\n");
    }
    printf("╚══════════════════════════════════════════╝\n" COLOR_RESET "\n");

    if (argc < 2) {
        print_error("Usage: novalang <filename.nova>");
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        print_error_var("Cannot open source file", argv[1]);
        return 1;
    }

    push_scope();

    if (yyparse() == 0 && root_ast) {
        eval_ast(root_ast);
        printf("\n");
        print_success("Program executed successfully.");
        free_ast(root_ast);
    } else {
        printf("\n");
        print_error("Execution aborted due to parsing error.");
    }

    free_symbol_table();
    fclose(yyin);
    return 0;
}
