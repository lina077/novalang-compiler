# NovaLang - Custom Programming Language Interpreter

NovaLang is a lightweight, custom domain-specific programming language developed using **Flex (Lexical Analyzer)**, **Bison (Yacc Parser)**, and **C**. It features abstract syntax tree (AST) construction, symbol table management, dynamic evaluation, and semantic checking.

## 🚀 Features
- **Custom Syntax & Extension**: `.nova`
- **Variable Handling**: Declaration and dynamic value assignments.
- **Arithmetic & Logical Operators**: `+`, `-`, `*`, `/`, `>`, `<`, `==`, `!=`, etc.
- **Control Flow**: Conditional logic (`if`, `else`) and Loops (`while`, `for`).
- **Functions & Recursion**: Support for function definition, invocation, and recursive algorithms.
- **Error Handling**: Detailed lexical, syntax, and semantic error reporting with line numbers.

## 📁 Repository Structure
- `lexer.l`: Flex lexical analyzer definition.
- `parser.y`: Bison grammar and AST construction rules.
- `ast.c` / `ast.h`: Abstract Syntax Tree definitions and tree operations.
- `symbol_table.c` / `symbol_table.h`: Variable and scope tracking system.
- `eval.c` / `eval.h`: Interpreter execution and tree traversal logic.
- `main.c`: Entry point for reading and parsing `.nova` files.
- `Makefile`: Automated compilation and build system.
- `*.nova`: 15 comprehensive test scripts covering language constructs.

## 🛠️ Build and Execution
```bash
# Build the interpreter
make

# Run a sample program
./novalang variables.nova

# Run all test scripts sequentially
make clean && make
