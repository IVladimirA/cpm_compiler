#include <iostream>
#include "mixed.h"
#include "utils.h"
#include "node.h"
#include "parser/node.grammar.h"
#include "parser/node.lexer.h"

void yyerror(const char *error) {
    std::cerr << error;
}

int yywrap() {
    return 1;
}

int main(void) {
    Mixed a = Mixed(1323.1);
    Mixed b = Mixed(54) + Mixed(13);
    Mixed c = Mixed("12.3");
    //std::cout << a + b << std::endl << b + c << std::endl << c - a << std::endl;
    print(c - a);
    return 0;
}