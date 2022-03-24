%{

#include <iostream>
#include <string>

%}

%option outfile="lex.c" header-file="lex.h"

blank [ \t\r\n]+
digit [0-9]
letter [a-zA-Z]
const_decl val
var_decl var
print print
input input
string "({letter}|{digit}|_|{blank})"
integer ({digit})*
float ({digit})*.({digit})*
variable {letter}({letter}|{digit}|"_")*

%%

{variable} {
    yylval.name=new std::string(yytext);
    std::cout << "variable" << std::endl;
    return VAR;
}

{string} {
    yylval.name=new std::string(yytext);
    std::cout << "string" << std::endl;
    return STRING;
}

{integer} {
    yylval.name=new std::string(yytext);
    std::cout << "integer" << std::endl;
    return INT;
}

{float} {
    yylval.name=new std::string(yytext);
    std::cout << "float" << std::endl;
    return FLOAT;
}

{const_decl} {
    yylval.name=new std::string(yytext);
    std::cout << "const_decl" << std::endl;
    return CONST_D;
}

{var_decl} {
    yylval.name=new std::string(yytext);
    std::cout << "var_decl" << std::endl;
    return VAR_D;
}

{print} {
    yylval.name=new std::string(yytext);
    std::cout << "print" << std::endl;
    return PRINT;
}

{input} {
    yylval.name=new std::string(yytext);
    std::cout << "input" << std::endl;
    return INPUT;
}

"(" return LEFT_BRACKET;
")" return RIGHT_BRACKET;
"=" return OP_EQUATION;
"+" return OP_PLUS;
"-" return OP_MINUS;
";" return COMMAND_END;

%%