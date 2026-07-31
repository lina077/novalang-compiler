%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"
%}

%option noyywrap
%option yylineno

DIGIT   [0-9]
LETTER  [a-zA-Z_]
ID      {LETTER}({LETTER}|{DIGIT})*
NUMBER  {DIGIT}+(\.{DIGIT}+)?
STRING  \"([^\\\"]|\\.)*\"

%%

[ \t\r\n]+          ; /* Ignore whitespaces */
"//".*              ; /* Ignore single-line comments */

"var"               { return VAR; }
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"fn"                { return FN; }
"return"            { return RETURN; }
"print"             { return PRINT; }
"input"             { return INPUT; }

"=="                { return EQ; }
"!="                { return NE; }
">="                { return GE; }
"<="                { return LE; }
">"                 { return GT; }
"<"                 { return LT; }
"&&"                { return AND; }
"||"                { return OR; }
"!"                 { return NOT; }

"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return MUL; }
"/"                 { return DIV; }
"="                 { return ASSIGN; }

";"                 { return SEMICOLON; }
","                 { return COMMA; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }

{NUMBER} {
    yylval.fval = atof(yytext);
    return NUMBER;
}

{STRING} {
    // Strip quotes
    int len = strlen(yytext);
    char *str = malloc(len - 1);
    strncpy(str, yytext + 1, len - 2);
    str[len - 2] = '\0';
    yylval.sval = str;
    return STRING;
}

{ID} {
    yylval.sval = strdup(yytext);
    return IDENTIFIER;
}

. {
    fprintf(stderr, "Unknown character '%s' at line %d\n", yytext, yylineno);
}

%%
