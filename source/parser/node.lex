%{

#include <iostream>
#include <string>
#include "../node.h"
#include "node.grammar.h"

%}

%option outfile="lex.c" header-file="lex.h"

blank [ \t\r\n]+
digit [0-9]
letter [a-zA-Z]
integer {digit}+
float {digit}+"."{digit}*
variable {letter}({letter}|{digit}|"_")*
string """({letter}|{digit}|"_"|{blank})*"""

%%

"var" {
    yylval.name=new std::string(yytext);
    std::cout << "var_decl" << std::endl;
    return VAR_D;
}

"val" {
    yylval.name=new std::string(yytext);
    std::cout << "const_decl" << std::endl;
    return CONST_D;
}

"print" {
    yylval.name=new std::string(yytext);
    std::cout << "print" << std::endl;
    return PRINT;
}

"input" {
    yylval.name=new std::string(yytext);
    std::cout << "input" << std::endl;
    return INPUT;
}

{string} {
    yylval.name=new std::string(yytext);
    std::cout << "string" << std::endl;
    return STRING_L;
}

{variable} {
    yylval.name=new std::string(yytext);
    std::cout << "variable" << std::endl;
    return VAR;
}

{integer} {
    yylval.name=new std::string(yytext);
    std::cout << "integer" << std::endl;
    return INT_L;
}

{float} {
    yylval.name=new std::string(yytext);
    std::cout << "float" << std::endl;
    return FLOAT_L;
}

"(" return LEFT_BRACKET;
")" return RIGHT_BRACKET;
"=" return OP_EQUATION;
"+" return OP_PLUS;
"-" return OP_MINUS;
";" return COMMAND_END;

%%