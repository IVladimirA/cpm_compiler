#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include "node/node.h"
#include "parser/node.tab.h"
#include "parser/node.lexer.h"

// Program's parsed commands
extern std::vector<const Node*> code;

// Print program usage
static void print_usage(const std::string& program_name) {
    std::cout << "Usage: " << program_name << " FILE [DIRECTORY]\n"
        << "Options:\n"
        << "\t-h, --help\tShow this help message\n"
        << "Specify the path to .cpm source FILE to create DIRECTORY with a.cpp and a.out files. Directory is \"./out\" by default.\n";
}

// Parse code of source file
static void parse_code(std::ifstream& in_file) {
    std::stringstream all_code;
    all_code << in_file.rdbuf();
    yy_scan_string(all_code.str().c_str());
    yyparse();
}

// Open source file and run parser
static void parse_file(const std::string& file_path) {
    std::ifstream in_file(file_path);
    parse_code(in_file);
    in_file.close();
}

// Generate code and write it to .cpp file
static int generate_cpp(std::ostream& out_file) {
    std::array<int, 4> errors; // Stores counts of errors
    errors.fill(0);
    // 0 - redeclaration of constant
    // 1 - redeclaration of variable
    // 2 - usage of undefined identifier
    // 3 - redifinition of constant
    
    
    std::unordered_set<std::string> consts; // Identifiers of defined constants
    std::unordered_set<std::string> vars_defined; // Identifiers of defined variables
    std::unordered_set<std::string> vars_declared; // Identifiers of declared variables

    // Error checking, code generating and writing
    out_file << "#include \"../mixed.h\"\n\n";
    out_file << "int main() {";
    int wrong_commands = 0; // Count of commands with errors
    for (const Node* command : code) {
        if (command->check_statement(consts, vars_declared, vars_defined, errors)) {
            ++wrong_commands;
        } else
            out_file << command->generate_statement();
        delete command;
    }
    out_file << "\nreturn 0;\n}\n";

    // Output of failure message
    if (wrong_commands > 0) {
        std::cout << "Compilation failed"
            << "\nCommand(s) with errors: " << wrong_commands
            << "\nRedeclaration(s) of constant: " << errors[0]
            << "\nRedeclaration(s) of variable: " << errors[1]
            << "\nUsage(s) of undefined identifier: " << errors[2]
            << "\nRedifinition(s) of constant: " << errors[3] << "\n";
        return 1;
    }
    return 0;
}

// Open .cpp file and run code generating
static int write_cpp(const std::string& file_path) {
    std::ofstream out_file(file_path);
    const int return_code = generate_cpp(out_file);
    out_file.close();
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
    const std::string out_path = (argc == 3) ? argv[2] : ".";  // Resulting directory with .cpp and .out files location

    const size_t path_ending = program_name.rfind("/");
    const std::string compiler_path = (path_ending == std::string::npos)
        ? "./" : program_name.substr(0, path_ending + 1);  // Path to C+- compiler

    parse_file(source_path); // Parsing source code
    system(("mkdir -p " + compiler_path + "out").c_str()); // Creating "out" directory
    const int return_code = write_cpp(compiler_path + "out/a.cpp"); // Writing transpiled code to .cpp file
    if (return_code != 0) {
        system(("rm -rf " + compiler_path + "out").c_str());
        return 1;
    }

    // Compiling a.cpp into a.out file
    system(("g++ " + compiler_path + "out/a.cpp " + compiler_path + "libmixed.a -o" + compiler_path + "out/a.out").c_str());
    std::cout << compiler_path << "out/a.out successfully compiled\n";

    // Moving out directory to destination location
    if (system(("mv " + compiler_path + "out " + out_path).c_str()) == 0) {
        std::cout << compiler_path << "out successfully moved to " << out_path << "\n";
        return 0;
    }
    std::cout << "Couldn't moved " << compiler_path << "out to " << out_path << "\n";
    return 1;
}