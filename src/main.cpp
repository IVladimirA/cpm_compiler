#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include "node/node.h"
#include "parser/node.tab.h"
#include "parser/node.lexer.h"

extern std::vector<Node*> code;

void parse_code(std::ifstream& in_file) {
    std::string line;
    while (std::getline(in_file, line)) {
        yy_scan_string(line.c_str());
        yyparse();
    }
}

void parse_file(std::string& file_path) {
    std::ifstream infile(file_path);
    parse_code(infile);
    infile.close();
}

void generate_cpp(std::ostream& outfile) {
    std::array<int, 4> errors; // 0 - redeclaration of const, 1 - redeclaration of variable, 2 - usage of undefined identifier, 3 - redifinition of const
    errors.fill(0);
    std::unordered_set<std::string> consts;
    std::unordered_set<std::string> vars_defined;
    std::unordered_set<std::string> vars_declared;
    outfile << "#include \"mixed.h\"\n\n";
    outfile << "int main() {\n";
    for (Node* line : code) {
        if (line->check_line(consts, vars_declared, vars_defined, errors))
            continue;
        outfile << '\t' << line->generateLine() << ";\n";
        delete line;
    }
    outfile << "\treturn 0;\n}\n";
    outfile << "/*\nErrors:\n";
    outfile << "Redeclaration of constant: " << errors[0] << "\n";
    outfile << "Redeclaration of variable: " << errors[1] << "\n";
    outfile << "Usage of undefined identifier: " << errors[2] << "\n";
    outfile << "Redifinition of constant: " << errors[3] << "\n*/";
}

void write_cpp(std::string& file_path) {
    std::ofstream outfile(file_path);
    generate_cpp(outfile);
    outfile.close();
}

int main(int argc, char** argv) {
    if (argc < 2)
        throw std::invalid_argument("No input file");
    std::string in_file_path = argv[1];
    std::string cpp_file_path = "a.cpp";
    std::string out_file_path = "a.o";
    std::string library_path = "libmixed.so";
    if (argc >= 3)
        out_file_path = argv[2];
    if (argc >= 4)
        library_path = argv[3];
    if (argc >= 5)
        cpp_file_path = argv[4];
    parse_file(in_file_path);
    write_cpp(cpp_file_path);
    system(("g++ " + cpp_file_path + " " + library_path + " -o" + out_file_path).c_str());
    return 0;
}