#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include "node/node.h"
#include "parser/node.tab.h"
#include "parser/node.lexer.h"

extern std::vector<Node*> code;

static void parse_code(std::ifstream& in_file) {
    std::string line;
    while (std::getline(in_file, line)) {
        yy_scan_string(line.c_str());
        yyparse();
    }
}

static void parse_file(const std::string& file_path) {
    std::ifstream infile(file_path);
    parse_code(infile);
    infile.close();
}

static void generate_cpp(std::ostream& outfile) {
    std::array<int, 4> errors; // 0 - redeclaration of const, 1 - redeclaration of variable, 2 - usage of undefined identifier, 3 - redifinition of const
    errors.fill(0);
    std::unordered_set<std::string> consts;
    std::unordered_set<std::string> vars_defined;
    std::unordered_set<std::string> vars_declared;
    outfile << "#include \"../mixed.h\"\n\n";
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

static void write_cpp(const std::string& file_path) {
    std::ofstream outfile(file_path);
    generate_cpp(outfile);
    outfile.close();
}

int main(int argc, char** argv) {
    std::cout << argv[0] << std::endl;
    if (argc < 2)
        throw std::invalid_argument("No input file");
    std::string in_file_path = argv[1];
    std::string cpm_path = argv[0];
    const size_t dir_path = cpm_path.rfind("/");
    if (dir_path != std::string::npos)
        cpm_path = cpm_path.substr(0, dir_path + 1);
    else
        cpm_path = "./";
    parse_file(in_file_path);
    system(("mkdir -p " + cpm_path + "out").c_str());
    write_cpp(cpm_path + "out/a.cpp");
    system(("g++ " + cpm_path + "out/a.cpp " + cpm_path + "libmixed.so -o" + cpm_path + "out/a.out").c_str());
    return 0;
}