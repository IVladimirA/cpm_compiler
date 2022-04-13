#include <iostream>
#include <fstream>
#include <sstream>
#include "node/node.h"
#include "parser/node.tab.h"
#include "parser/node.lexer.h"
#include "visitor/visitor.h"

extern Root* root;

// Print program usage
static void print_usage(const std::string& program_name) {
    std::cout << "Usage: " << program_name << " FILE [DIRECTORY]\n"
        << "Options:\n"
        << "\t-h, --help\tShow this help message\n"
        << "Specify the path to .cpm source FILE to create DIRECTORY with a.cpp and a.out files. Directory is \"./out\" by default.\n";
}

// Parse code of source file
static bool parse_code(std::ifstream& in_file) {
    std::stringstream all_code;
    all_code << in_file.rdbuf();
    yy_scan_string(all_code.str().c_str());
    return yyparse() == 0;
}

// Open source file and run parser
static bool parse_file(const std::string& file_path) {
    std::ifstream in_file(file_path);
    return parse_code(in_file);
}

static bool check_code(const Node* tree) {
    CodeChecker checker = CodeChecker();
    tree->accept(checker);
    if (!checker.errors.empty()) {
        for (const std::string& e : checker.errors) {
            std::cout << e << '\n';
        }
        return false;
    }
    return true;
}

static std::string generate_cpp(const Node* tree) {
    std::string code =  "#include \"../mixed.h\"\n";
    code += "int main() {";
    CodeGenerator writer = CodeGenerator();
    tree->accept(writer);
    code += writer.get_code();
    code += "\nreturn 0;\n}\n";
    return code;
}

static void check_argc(int argc, const std::string& program_name) {
    if (argc < 2)
        throw std::invalid_argument("No input file. Try '" + program_name + " --help' for more information.");
    if (argc > 3)
        throw std::invalid_argument("Too many arguments: " + std::to_string(argc - 1) + " given. Try '" + program_name + " --help' for more information.");
}

int main(int argc, char** argv) {
    const std::string program_name = argv[0];
    check_argc(argc, program_name);

    const std::string source_path = argv[1]; // .cpm source file location
    if (source_path == "-h" || source_path == "--help") {
        print_usage(program_name);
        return 0;
    }
    const std::string out_path = (argc == 3) ? argv[2] : ".";  // Resulting directory with .cpp and .out files location

    const size_t path_ending = program_name.rfind('/');
    const std::string compiler_path = (path_ending == std::string::npos)
        ? "./" : program_name.substr(0, path_ending + 1);  // Path to C+- compiler

    if (!parse_file(source_path)) { // Parsing source code
        std::cout << "\nParsing of " << source_path << " failed\n";
        return 1;
    }
    system(("mkdir -p " + compiler_path + "out").c_str()); // Creating "out" directory
    if (!check_code(root)) {
        std::cout << "Compilation of " << source_path << " failed\n";
        return 1;
    }
    
    std::ofstream out_file(compiler_path + "out/a.cpp");
    out_file << generate_cpp(root); // Writing transpiled code to .cpp file
    out_file.close();

    // Compiling a.cpp into a.out file
    if (system(("g++ " + compiler_path + "out/a.cpp " + compiler_path + "libmixed.a -o" + compiler_path + "out/a.out").c_str()) != 0) {
        //system(("rm -rf " + compiler_path + "out").c_str());
        std::cout << "Compilation of a.cpp failed\n";
        return 1;
    }
    std::cout << compiler_path << "out/a.out successfully compiled\n";
    if (out_path + "/" == compiler_path) {
        return 0;
    }
    // Moving out directory to destination location
    if (system(("mv " + compiler_path + "out " + out_path).c_str()) != 0) {
        std::cout << "Couldn't move " << compiler_path << "out to " << out_path << '\n';
        return 1;
    }
    std::cout << compiler_path << "out successfully moved to " << out_path << '\n';
    return 0;
}