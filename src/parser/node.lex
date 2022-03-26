%{

#include <iostream>
#include <string>
#include "../node/node.h"
#include "node.grammar.h"

%}

%option outfile="lex.c" header-file="lex.h"

blank [ \t\r\n]+
digit [0-9]
letter [a-zA-Z]
integer {digit}+
float {digit}+"."{digit}*
variable {letter}({letter}|{digit}|"_")*
string \042({letter}|{digit}|"_"|{blank})*\042

%%

"var" {
    yylval.name=new std::string(yytext);
    return VAR_D;
}

"val" {
    yylval.name=new std::string(yytext);
    return CONST_D;
}

"print" {
    yylval.name=new std::string(yytext);
    return PRINT;
}

"input" {
    yylval.name=new std::string(yytext);
    return INPUT;
}

{string} {
    yylval.name=new std::string(yytext);
    return STRING_L;
}

{variable} {
    yylval.name=new std::string(yytext);
    return VAR;
}

{integer} {
    yylval.name=new std::string(yytext);
    return INT_L;
}

{float} {
    yylval.name=new std::string(yytext);
    return FLOAT_L;
}

"(" return LEFT_BRACKET;
")" return RIGHT_BRACKET;
"=" return OP_EQUATION;
"+" return OP_PLUS;
"-" return OP_MINUS;
";" return COMMAND_END;
{blank}

%%