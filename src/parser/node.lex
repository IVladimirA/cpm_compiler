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

{blank} ;
"(" return LEFT_BRACKET;
")" return RIGHT_BRACKET;
"=" return OP_EQUATION;
"+" return OP_PLUS;
"-" return OP_MINUS;
";" return COMMAND_END;

%%