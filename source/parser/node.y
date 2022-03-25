%{

#include <iostream>
#include <string>
#include <vector>
#include "../node.h"

int yylex(void);
void yyerror(const char* msg);
std::vector<Mixed> res;

%}

%union {
    Node* node;
    std::string* name;
}

%token <name> VAR INT_L FLOAT_L STRING_L
%token VAR_D CONST_D
%token OP_PLUS OP_MINUS OP_EQUATION
%token COMMAND_END
%token LEFT_BRACKET RIGHT_BRACKET
%token PRINT INPUT

%right OP_EQUATION
%left OP_PLUS
%left OP_MINUS

%type <node> Input Line Expression Declaration

%start Input

%%

Input: Line { }
| Input Line { }
;

Line: Expression COMMAND_END { $$ = new Node(); }
| Declaration OP_EQUATION Expression COMMAND_END { $$ = new Node(); }
| VAR OP_EQUATION Expression COMMAND_END { $$ = new Node(); }
;

Expression: Expression OP_PLUS Expression { $$ = new Node(); }
| Expression OP_MINUS Expression { $$ = new Node(); }
| PRINT LEFT_BRACKET Expression RIGHT_BRACKET { $$ = new Node(); }
| INPUT LEFT_BRACKET Expression RIGHT_BRACKET { $$ = new Node(); }
| VAR { $$ = new Node(); }
| INT_L { $$ = new Node(); }
| FLOAT_L { $$ = new Node(); }
| STRING_L { $$ = new Node(); }
;

Declaration: VAR_D VAR { $$ = new Node(); }
| CONST_D VAR { $$ = new Node(); }
;

%%