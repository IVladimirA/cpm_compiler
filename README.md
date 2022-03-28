# C+- toy language parser and compiler
## Project description
Flex (Fast Lexical Analyzer Generator) and Bison (parser generator) are being used to parse C+- toy language source file `.cpm` and create abstract syntax tree (AST). Then corresponding C++ file `.cpp` is generated and compiled into executable.
## C+- description
- C+- consists only of
    1. Variables `var a;` and constants `val b`
    2. Numeric and string literals (`43`, `12.3`, `"my string"`, etc)
    3. Addition, subtraction and assignment operators `+`, `-`, `=`
    4. Output function `print()`
    5. Input function `input()`
    6. Single line and multi-line comments
- There are only 3 types of values
    1. Int - 8 byte
    2. String - sequence of characters
    3. Float - 8 byte
- C+- is an implicit dynamic typization language
    1. Same variable can store numbers and strings, the following is correct
        ```
        var a = 5.5;
        a = "Hello";
        a = 34.5;
        ```
    2. Operators follow rules (order of arguments doesn't matter)
        - String + Int/Float: Numeric converts to string
        - String - Int/Float: String converts to Int/Float, if it's not possible String value considered to be `0`
        - Int/Float + Int/Float: Int converts to Float if necessary
- Code example
    ```
    var str = "Hello"; // variable declared using var
    str = str + "1";   // variables can be changed
    print(str);        // print function takes only one argument

    str = 21 + 2 + 18; // same variable can store numbers
    print(str);        // -> 41

    val worldString = "World"; // constant declared using val
    val answerNumber = 42;

    print(worldString + answerNumber); // -> World42
    print(worldString - answerNumber); // -> -42

    val pi = 3.14;
    print(pi + answerNumber); // -> 45.14

    val numberString = "134";
    print(numberString + answerNumber); // -> 92
    print(numberString - answerNumber); // -> 13442

    val name = input("Name: "); // prints "Name: " and waits for input
    print("Your name is " + name);
    ```
    Output
    ```
    Hello1
    41
    World42
    -42
    45.14
    92
    13442
    Name: Vladimir
    Your name is Vladimir
    ```


    
## Setup
Clone repository
```
https://github.com/IVladimirA/cpm_compiler.git
```
Create `cpm_compiler` folder with compiler and library
```
$ make
```
Transpile `example/a.cpm` to `out/a.cpp` and then compile to `out/a.out` and run it
```
$ make run
```
Source file can be changed by changing example path in Makefile
```
example := examples/addition.cpm
```
All source files and Makefile can be archived
```
$ make pack
```
Compiler can be used without make, but it requires files from `cpm_compiler` directory and `g++` compiler installed
```
$ cpm_compiler/c+- --help
Usage: cpm_compiler/c+- FILE [DIRECTORY]
Options:
        -h, --help      Show this help message
Specify the path to .cpm source FILE to create DIRECTORY with a.cpp and a.out files. Directory is "./out" by default.
```
## Parsing Example
Running Makefile
```
$ pwd
/home/vladimir/projects/cpm_compiler
```
```
$ make run
mkdir -p cpm_compiler
flex src/parser/node.lex
mv lex.c src/parser/node.lexer.c
mv lex.h src/parser/node.lexer.h
bison -d -v src/parser/node.y -o src/parser/node.tab.c
g++ src/main.cpp src/node/node.h src/node/node.cpp src/parser/node.lexer.c src/parser/node.tab.c -std=c++17 -Wall -O2 -w -o cpm_compiler/c+-
cp src/mixed/mixed.h cpm_compiler/mixed.h
g++ src/mixed/mixed.cpp -std=c++17 -Wall -O2 -fPIC -c -o cpm_compiler/mixed.o
g++ cpm_compiler/mixed.o -fPIC -shared -o cpm_compiler/libmixed.so
rm -f cpm_compiler/mixed.o
cpm_compiler/c+- examples/perimeter.cpm
out/a.out
Calculating perimeter of rectangle
Enter length of one side: 5.5
Enter length of another side: 123.4
Perimeter: 5.55.5123.4123.4
Oops... strings concatenated
Let's try another time!
Perimeter: 257.8
```
Running `c+-` compiler directly
```
$ pwd
/home/vladimir/projects/cpm_compiler
```
```
$ make
mkdir -p cpm_compiler
flex src/parser/node.lex
mv lex.c src/parser/node.lexer.c
mv lex.h src/parser/node.lexer.h
bison -d -v src/parser/node.y -o src/parser/node.tab.c
g++ src/main.cpp src/node/node.h src/node/node.cpp src/parser/node.lexer.c src/parser/node.tab.c -std=c++17 -Wall -O2 -w -o cpm_compiler/c+-
cp src/mixed/mixed.h cpm_compiler/mixed.h
g++ src/mixed/mixed.cpp -std=c++17 -Wall -O2 -fPIC -c -o cpm_compiler/mixed.o
g++ cpm_compiler/mixed.o -fPIC -shared -o cpm_compiler/libmixed.so
rm -f cpm_compiler/mixed.o
```
```
$ cpm_compiler/c+- examples/perimeter.cpm
```
```
$ out/a.out
Calculating perimeter of rectangle
Enter length of one side: 5.5
Enter length of another side: 123.4
Perimeter: 5.55.5123.4123.4
Oops... strings concatenated
Let's try another time!
Perimeter: 257.8
```
Running `c+-` compiler from another location (with source `examples/errors.cpm`)
```
$ pwd
/home/vladimir/projects/cpm_compiler
$ mkdir ../test
$ cd ../test
$ pwd
/home/vladimir/projects/test
```
```
$ ../cpm_compiler/cpm_compiler/c+- ../cpm_compiler/examples/errors.cpm ./result
$ ls -R
.:
result

./result:
a.cpp  a.out
```
```
$ result/a.out
10
```
### perimeter.cpm source file
```
print("Calculating perimeter of rectangle");
val a = input("Enter length of one side: ");
val b = input("Enter length of another side: ");
var answer = a + a + b + b;
print("Perimeter: " + answer);
print("Oops... strings concatenated");
val a_numeric = a - 0;
val b_numeric = b - 0;
answer = a_numeric + a_numeric + b_numeric + b_numeric;
print("Let's try another time!");
print("Perimeter: " + answer);
```
### a.cpp transpiled file
```
#include "../mixed.h"

int main() {

print(Mixed("Calculating perimeter of rectangle"));
const Mixed a = input(Mixed("Enter length of one side: "));
const Mixed b = input(Mixed("Enter length of another side: "));
Mixed answer = a + a + b + b;
print(Mixed("Perimeter: ") + answer);
print(Mixed("Oops... strings concatenated"));
const Mixed a_numeric = a - Mixed(0);
const Mixed b_numeric = b - Mixed(0);
answer = a_numeric + a_numeric + b_numeric + b_numeric;
print(Mixed("Let's try another time!"));
print(Mixed("Perimeter: ") + answer);
return 0;
}
/*
Commands skipped: 0
Errors:
Redeclaration of constant: 0
Redeclaration of variable: 0
Usage of undefined identifier: 0
Redifinition of constant: 0
*/
```
### a.out execution output
```
Calculating perimeter of rectangle
Enter length of one side: 5.5
Enter length of another side: 123.4
Perimeter: 5.55.5123.4123.4
Oops... strings concatenated
Let's try another time!
Perimeter: 257.8
```
## Requirements
- `g++` >= 9.3.0
- `flex` >= 2.6.4
- `bison` >= 3.5.1
- `make` >= 4.2.1