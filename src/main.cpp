#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include "mixed/mixed.h"
#include "utils/utils.h"
#include "node/node.h"
#include "parser/node.tab.h"
#include "parser/node.lexer.h"

extern std::vector<Node*> code;

int main(int argc, char** argv) {
    std::string line;
    std::string in_file_path = "examples/a.cpm";
    std::string out_file_path = "out/a.cpp";
    std::ifstream infile(in_file_path);
    std::array<int, 4> errors; // 0 - redeclaration of const, 1 - redeclaration of variable, 2 - usage of undefined identifier, 3 - redifinition of const
    errors.fill(0);
    std::unordered_set<std::string> consts;
    std::unordered_set<std::string> vars_defined;
    std::unordered_set<std::string> vars_declared;
    while (std::getline(infile, line)) {
        yy_scan_string(line.c_str());
        yyparse();
    }
    std::ofstream outfile(out_file_path);
    outfile << "#include \"../src/mixed/mixed.h\"\n";
    outfile << "#include \"../src/utils/utils.h\"\n\n";
    outfile << "int main(void) {\n";
    for (Node* line : code) {
        if (line->check_line(consts, vars_declared, vars_defined, errors))
            continue;
        outfile << '\t' << line->generateLine() << ";\n";
    }
    outfile << "\treturn 0;\n}\n";
    outfile << "/*\nErrors:\n";
    outfile << "Redeclaration of constant: " << errors[0] << "\n";
    outfile << "Redeclaration of variable: " << errors[1] << "\n";
    outfile << "Usage of undefined identifier: " << errors[2] << "\n";
    outfile << "Redifinition of constant: " << errors[3] << "\n*/";
    return 0;
}