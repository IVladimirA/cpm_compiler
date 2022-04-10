#include <iostream>
#include <fstream>
#include <sstream>
#include "node/node.h"
#include "error/error.h"
#include "parser/node.tab.h"
#include "parser/node.lexer.h"
#include "visitor/visitor.h"

// Вот это лучше бы превратить в узел типа Root. См. комментарии дальше.
// Program's parsed commands
extern std::vector<const Node*> code;

// Print program usage
static void print_usage(const std::string& program_name) {
    std::cout << "Usage: " << program_name << " FILE [DIRECTORY]\n"
        << "Options:\n"
        << "\t-h, --help\tShow this help message\n"
        << "Specify the path to .cpm source FILE to create DIRECTORY with a.cpp and a.out files. Directory is \"./out\" by default.\n";
}

// Я бы, конечно, вынес бы логику парсинга в отдельный класс,
// но, возможно, здесь это не критично.
//
// Однако я не совсем понимаю необходимость в разделении парсинга
// на две функции.
// Функция parse_code используется один раз и ее название в целом
// не соответствует ее логике. Логичнее было бы если бы функция
// парсила строку, тогда она была бы универсальной. Однако в
// любом случае смысла в разделении я не вижу.

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
    // Закрывать поток явно не нужно!
    // Почитай про RAII.
    // https://stackoverflow.com/questions/748014/do-i-need-to-manually-close-an-ifstream
    in_file.close();
}

// Лучше бы если класс CodeGenerator включал в себя все необходимое
// и в качестве интерфейса давал одну функцию generate.
//
// Функция generate_cpp почему-то проверяет код на корректность.
// Проверка корректности должна выполняться отдельно и до генерации кода.
// Generate code and write it to .cpp file
static int generate_cpp(std::ostream& out_file) {
    bool seen_error = false;
    std::vector<const Error*> errors;
    std::unordered_set<std::string> consts; // Identifiers of defined constants
    std::unordered_set<std::string> vars_defined; // Identifiers of defined variables
    std::unordered_set<std::string> vars_declared; // Identifiers of declared variables

    // Error checking, code generating and writing
    out_file << "#include \"../mixed.h\"\n\n";
    out_file << "int main() {";

    // Упс, кажется эта переменная не используется :)
    int wrong_statements = 0; // Number of statements with errors

    // Странная архитектура. Почему эта строка идет извне?
    // Это часть реализации CodeGenerator и должна быть его полем.
    // А соответственно, когда ты хочешь с ней как-то работать, то ты
    // должен получить ее из CodeGenerator.
    //   writer->getCurrentLine();
    std::string* current_line = new std::string("");
    CodeGenerator writer = CodeGenerator(current_line);

    // Странная архитектура. А какой смысл передавать эти переменные туда?
    // У тебя они должны быть частью реализации CodeChecker и когда ты хочешь получить
    // какое-то поле, то ты получаешь его из CodeChecker.
    //   checker->getErrors();
    CodeChecker checker = CodeChecker(&errors, &consts, &vars_declared, &vars_defined);

    // Вот тут появляется проблема из-за того, что у тебя нет узла типа Root.
    // В идеале с ним код должен выглядеть так:
    //   root->accept(checker)
    //   root->accept(writer)
    for (const Node* command : code) {
        // Довольно странно сразу и проверять и генерить код. Лучше было
        // сначала все проверить, если ошибок не нашлось, то запускать
        // кодогенерацию.
        command->accept(checker);
        command->accept(writer);

        // Вот это задача самого CodeChecker, а не кода который его вызывает!
        if (errors.size() > 0) { // Clang-Tidy: The 'empty' method should be used to check for emptiness instead of 'size'
            seen_error = true;
            // Не вижу большого смысла делать ошибки указателями (тем более сырыми, см. unique_ptr и shared_ptr!!)
            // У тебя ошибка это всего-лишь строка, ее проще копировать, чем делать выделения в куче.
            // Излишняя оптимизация.
            for (const Error* e : errors) {
                std::cout << "Error: " << e->get_message() << "\n";
                // Которая требует такого ужаса. И потенциально может быть утечкой памяти.
                // Ты забыл виртуальный деструктор.
                // https://stackoverflow.com/questions/69081119/that-is-abstract-but-has-non-virtual-destructor-the-delete-is-causing-an-error
                delete e; // Delete called on 'Error' that is abstract but has non-virtual destructor
            }
            std::cout << "In command:";
            std::cout << *current_line << "\n";
        }
        errors.clear();

        // Вот это я вообще не понимаю частно говоря, понятно что ты пишешь
        // в файл строку, но это должно делаться в CodeGenerator (это его обязанность!)
        // а не здесь,
        out_file << *current_line;
        std::string* old_line = current_line;
        current_line = new std::string("");
        delete old_line;
        writer.update_string(current_line);
        // Ты забыл виртуальный деструктор.
        delete command; // Delete called on 'Node' that is abstract but has non-virtual destructor
    }
    out_file << "\nreturn 0;\n}\n";
    delete current_line;
    if (seen_error) {
        return 1;
    }
    return 0;
}

// Вот это тоже было бы лучше в составе класса CodeGenerator.
// Open .cpp file and run code generating
static int write_cpp(const std::string& file_path) {
    std::ofstream out_file(file_path);
    const int return_code = generate_cpp(out_file);
    // Тут тоже не нужно закрывать явно.
    out_file.close();
    return return_code;
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

    const size_t path_ending = program_name.rfind("/"); // Clang-Tidy: 'rfind' called with a string literal consisting of a single character; consider using the more effective overload accepting a character
    const std::string compiler_path = (path_ending == std::string::npos)
        ? "./" : program_name.substr(0, path_ending + 1);  // Path to C+- compiler

    parse_file(source_path); // Parsing source code
    system(("mkdir -p " + compiler_path + "out").c_str()); // Creating "out" directory
    const int return_code = write_cpp(compiler_path + "out/a.cpp"); // Writing transpiled code to .cpp file
    if (return_code != 0) {
        system(("rm -rf " + compiler_path + "out").c_str());
        std::cout << "Compilation of " << source_path << " failed\n";
        return 1;
    }

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
    if (system(("mv " + compiler_path + "out " + out_path).c_str()) == 0) {
        std::cout << compiler_path << "out successfully moved to " << out_path << "\n";
        return 0;
    }
    std::cout << "Couldn't move " << compiler_path << "out to " << out_path << "\n";
    return 1;
}