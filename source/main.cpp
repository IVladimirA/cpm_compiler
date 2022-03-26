#include <iostream>
#include <vector>
#include <fstream>
#include "mixed.h"
#include "utils.h"
#include "node.h"
#include "parser/node.grammar.h"
#include "parser/node.lexer.h"

extern std::vector<Node*> code;

void yyerror(const char *error) {
    std::cerr << error;
}

int yywrap() {
    return 1;
}

int main(void) {
    std::string line;
    std::ifstream infile("input/a.cpm");
    while (std::getline(infile, line)) {
        yy_scan_string(line.c_str());
        yyparse();
    }
    /*
    Mixed a = Mixed(1323.1);
    Mixed b = Mixed(54) + Mixed(13);
    Mixed c = Mixed("12.3");
    std::cout << a + b << std::endl << b + c << std::endl << c - a << std::endl;
    print(c - a);
    */
    std::cout << '\n';
    for (Node* line : code) {
        std::cout << line->getStr() << ";\n";
    }
    return 0;
}