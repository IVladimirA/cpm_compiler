#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include "node/node.h"
#include "parser/node.tab.h"
#include "parser/node.lexer.h"

extern std::vector<const Node*> code;

static void print_usage(const std::string& program_name) {
    std::cout << "Usage: " << program_name << " FILE [DIRECTORY]\n"
              << "Options:\n"
              << "\t-h, --help\tShow this help message\n"
              << "Specify the path to .cpm source FILE to create DIRECTORY with a.cpp and a.out files. Directory is \"./out\" by default."
              << std::endl;
}

static void parse_code(std::ifstream& infile) {
    std::stringstream all_code;
    all_code << infile.rdbuf();
    yy_scan_string(all_code.str().c_str());
    yyparse();
}

static void parse_file(const std::string& file_path) {
    std::ifstream infile(file_path);
    parse_code(infile);
    infile.close();
}

static int generate_cpp(std::ostream& outfile) {
    std::array<int, 4> errors; // stores counts of errors
    errors.fill(0);
    /* 
        0 - redeclaration of constant
        1 - redeclaration of variable
        2 - usage of undefined identifier
        3 - redifinition of constant
    */
    
    std::unordered_set<std::string> consts; // identifiers of defined consts
    std::unordered_set<std::string> vars_defined; // identifiers of defined variables
    std::unordered_set<std::string> vars_declared; // identifiers of declared variables

    /* Printing code to .cpp file */

    outfile << "#include \"../mixed.h\"\n\n";
    outfile << "int main() {";
    int wrong_commands = 0; // Commands with errors count
    for (const Node* command : code) {
        if (command->check_command(consts, vars_declared, vars_defined, errors)) {
            ++wrong_commands;
        } else
            outfile << command->generate_command();
        delete command;
    }
    outfile << "\nreturn 0;\n}\n";
    if (wrong_commands > 0) {
        std::cout << "Compilation failed\n";
        std::cout << "Command(s) with errors: " << wrong_commands << "\n";
        std::cout << "Redeclaration(s) of constant: " << errors[0] << "\n";
        std::cout << "Redeclaration(s) of variable: " << errors[1] << "\n";
        std::cout << "Usage(s) of undefined identifier: " << errors[2] << "\n";
        std::cout << "Redifinition(s) of constant: " << errors[3] << "\n";
        return 1;
    }
    return 0;
}

static int write_cpp(const std::string& file_path) {
    std::ofstream outfile(file_path);
    const int return_code = generate_cpp(outfile);
    outfile.close();
    return return_code;
}

int main(int argc, char** argv) {
    const std::string program_name = argv[0];
    if (argc < 2)
        throw std::invalid_argument("No input file. Try '" + program_name + " --help' for more information.");
    if (argc > 3)
        throw std::invalid_argument("Too many arguments: " + std::to_string(argc - 1) + " given. Try '" + program_name + " --help' for more information.");

    const std::string source_path = argv[1]; // .cpm source file location
    if (source_path == "-h" || source_path == "--help") {
        print_usage(program_name);
        return 0;
    }
    const std::string out_path = (argc == 3) ? argv[2] : ".";  // resulting directory with .cpp and .out files location

    const size_t path_ending = program_name.rfind("/");
    const std::string compiler_path = (path_ending == std::string::npos)
        ? "./" : program_name.substr(0, path_ending + 1);  // path to compiler

    parse_file(source_path); // parsing source code
    system(("mkdir -p " + compiler_path + "out").c_str()); // creating "out" directory
    const int return_code = write_cpp(compiler_path + "out/a.cpp"); // writing transpiled code to .cpp file
    if (return_code != 0) {
        system(("rm -rf " + compiler_path + "out").c_str());
        return 1;
    }
    system(("g++ " + compiler_path + "out/a.cpp " + compiler_path + "libmixed.a -o" + compiler_path + "out/a.out").c_str()); // compiling a.cpp into a.out file
    std::cout << compiler_path << "out/a.out successfully compiled\n";
    if (system(("mv " + compiler_path + "out " + out_path).c_str()) == 0) {
        std::cout << compiler_path << "out successfully moved to " << out_path << "\n";
        return 0;
    }
    std::cout << "Couldn't moved " << compiler_path << "out to " << out_path << "\n";
}