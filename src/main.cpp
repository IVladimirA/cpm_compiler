#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include "node/node.h"
#include "parser/node.tab.h"
#include "parser/node.lexer.h"

extern std::vector<Node*> code;

static void print_usage(const std::string& name) {
    std::cout << "Usage: " << name << " file [directory]\n"
              << "Options:\n"
              << "\t-h, --help\tShow this help message\n"
              << "Specify the path to .cpm file to create directory with .cpp and .out files. Directory is \"./out\" by default."
              << std::endl;
}

static void parse_code(std::ifstream& in_file) {
    std::stringstream lines;
    lines << in_file.rdbuf();
    yy_scan_string(lines.str().c_str());
    yyparse();
}

static void parse_file(const std::string& file_path) {
    std::ifstream infile(file_path);
    parse_code(infile);
    infile.close();
}

static void generate_cpp(std::ostream& outfile) {
    std::array<int, 4> errors; // stores counts of errors
    errors.fill(0);
    /* 
        0 - redeclaration of const
        1 - redeclaration of variable
        2 - usage of undefined identifier
        3 - redifinition of const
    */
    
    std::unordered_set<std::string> consts; // identifiers of defined consts
    std::unordered_set<std::string> vars_defined; // identifiers of defined variables
    std::unordered_set<std::string> vars_declared; // identifiers of declared variables

    /* Printing code to .cpp file */

    outfile << "#include \"../mixed.h\"\n\n";
    outfile << "int main() {\n";
    int wrong_commands = 0; // Commands with errors count
    for (Node* command : code) {
        if (command->check_line(consts, vars_declared, vars_defined, errors)) {
            ++wrong_commands;
            continue;
        }
        outfile << command->generate_line() << "\n";
        delete command;
    }

    /* Printing error statisitic */

    outfile << "return 0;\n}\n";
    outfile << "/*\nCommands skipped: " << wrong_commands << "\n";
    outfile << "Errors:\n";
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
    std::string program_name = argv[0];
    if (argc < 2)
        throw std::invalid_argument("No input file. Try '" + program_name + " --help' for more information.");
    if (argc > 3)
        throw std::invalid_argument("Too many arguments: " + std::to_string(argc - 1) + " given. Try '" + program_name + " --help' for more information.");

    std::string source_path = argv[1]; // .cpm source file location
    std::string out_dir = ".";  // resulting directory with .cpp and .out files location
    if (argc == 3)
        out_dir = argv[2];
    if (source_path == "-h" || source_path == "--help") {
        print_usage(program_name);
        return 0;
    }
    
    std::string cpm_path = program_name;  // path to compiler
    const size_t path_ending = cpm_path.rfind("/");
    if (path_ending != std::string::npos)
        cpm_path = cpm_path.substr(0, path_ending + 1);
    else
        cpm_path = "./";

    parse_file(source_path); // parsing source code
    system(("mkdir -p " + cpm_path + "out").c_str()); // creating "out" directory
    write_cpp(cpm_path + "out/a.cpp"); // writing transpiled code to .cpp file

    system(("g++ " + cpm_path + "out/a.cpp " + cpm_path + "libmixed.so -o" + cpm_path + "out/a.out").c_str()); // compiling .cpp into .out file
    system(("cp -rf " + cpm_path + "out " + out_dir).c_str()); // moving "out" directory
    system(("rm -rf " + cpm_path + "out").c_str());
    return 0;
}