#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_CYAN    "\033[1;36m"

extern int yylineno;

static inline void print_error(const char *msg) {
    fprintf(stderr, COLOR_RED "[ERROR] Line %d: %s" COLOR_RESET "\n", yylineno, msg);
}

static inline void print_error_var(const char *msg, const char *var) {
    fprintf(stderr, COLOR_RED "[ERROR] Line %d: %s '%s'" COLOR_RESET "\n", yylineno, msg, var);
}

static inline void print_success(const char *msg) {
    printf(COLOR_GREEN "✔ %s" COLOR_RESET "\n", msg);
}

#endif
