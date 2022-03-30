%{

#include <iostream>
#include <string>
#include "../node/node.h"
#include "node.tab.h"

%}

%option outfile="lex.c" header-file="lex.h" noyywrap

blank [ \t\r\n]+
digit [0-9]
letter [a-zA-Z]
integer {digit}+
float {digit}+"."{digit}*
variable {letter}({letter}|{digit}|"_")*
string \042([^\042]|(\\\042))*\042
comment ({blank}?"//"[^\n]*)|({blank}?"/*"([^\n]|"\n")*"*/")

%%

"var" {
    yylval.name = new std::string(yytext);
    return t_variable_declaration;
}

"val" {
    yylval.name = new std::string(yytext);
    return t_constant_declaration;
}

"print" {
    yylval.name = new std::string(yytext);
    return t_print;
}

"input" {
    yylval.name = new std::string(yytext);
    return t_input;
}

{comment} {
    yylval.name = new std::string(yytext);
    return t_comment;
}

{string} {
    yylval.name = new std::string(yytext);
    return t_string_literal;
}

{variable} {
    yylval.name = new std::string(yytext);
    return t_variable;
}

{integer} {
    yylval.name = new std::string(yytext);
    return t_integer_literal;
}

{float} {
    yylval.name = new std::string(yytext);
    return t_float_literal;
}

{blank} ;
"(" return t_left_bracket;
")" return t_right_bracket;
"=" return t_equals;
"+" return t_plus;
"-" return t_minus;
";" return t_command_ending;

%%