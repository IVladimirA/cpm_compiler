#include <iostream>
#include <vector>
#include <fstream>
#include "mixed/mixed.h"
#include "utils/utils.h"
#include "node/node.h"
#include "parser/node.grammar.h"
#include "parser/node.lexer.h"

extern std::vector<Node*> code;

void yyerror(const char *error) {
    std::cerr << error;
}

int yywrap() {
    return 1;
}

int main(int argc, char** argv) {
    /*const Mixed a = Mixed(1323.1);
    Mixed b = Mixed(54) + Mixed(13);
    Mixed c = Mixed("12.3");
    c = "sd12ba4c";
    b = 99;
    print(c);
    print(c + a);
    print(c - a);
    print(c + b);
    print(c - b);*/
    std::string line;
    std::string in_file_path = "examples/a.cpm";
    std::ifstream infile(in_file_path);
    while (std::getline(infile, line)) {
        yy_scan_string(line.c_str());
        yyparse();
    }
    std::ofstream outfile("out/a.out");
    outfile << "#include \"../source/mixed.h\"\n";
    outfile << "#include \"../source/utils.h\"\n\n";
    for (Node* line : code) {
        outfile << line->generateLine() << ";\n";
    }
    return 0;
}