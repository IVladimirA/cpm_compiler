%{
#include <string>

%}

%option outfile="terminals.c" header-file="terminals.h"

blank [ \t\r\n]+
digit [0-9]
letter [A-Z]
const_decl "val"
var_decl "var"
print "print"
input "input"
string "\"({letter}|{digit}|"_")*\""
integer ({digit})*
float ({digit})*["."]+({digit})*
variable {letter}({letter}|{digit}|"_")*

%%

{var} {
    yylval.name=new std::string(yytext);
    return VAR;
}

{string} {
    yylval.name=new std::string(yytext);
    return STRING;
}

"(" return LEFT_BRACKET;
")" return RIGHT_BRACKET;
"=" return OP_EQUATION;
"+" return OP_PLUS;
"-" return OP_MINUS;
";" return OP_AND;

%%