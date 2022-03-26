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
    std::string line;
    std::string in_file_path = "examples/a.cpm";
    std::string out_file_path = "out/a.cpp";
    if (argc > 1) {
        in_file_path = argv[1];
    }
    std::ifstream infile(in_file_path);
    while (std::getline(infile, line)) {
        yy_scan_string(line.c_str());
        yyparse();
    }
    std::ofstream outfile(out_file_path);
    outfile << "#include \"../src/mixed/mixed.h\"\n";
    outfile << "#include \"../src/utils/utils.h\"\n\n";
    outfile << "int main(void) {\n";
    for (Node* line : code) {
        outfile << '\t' << line->generateLine() << ";\n";
    }
    outfile << "}\n";
    return 0;
}