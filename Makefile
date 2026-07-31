all: novalang

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

novalang: parser.tab.c lex.yy.c
	gcc -Wall -Wno-unused-function -g parser.tab.c lex.yy.c ast.c symbol_table.c eval.c main.c -o novalang -lfl

clean:
	rm -f novalang lumen *.o parser.tab.c parser.tab.h lex.yy.c
