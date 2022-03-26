%{

#include <iostream>
#include <string>
#include <vector>
#include "../node/node.h"

int yylex(void);
void yyerror(const char* msg);
std::vector<Node*> code;

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

%type <node> Input Line Expression Declaration Variable Literal

%start Input

%%

Input: Line { }
| Input Line { }
;

Line: Expression COMMAND_END { code.push_back($1); }
| Declaration OP_EQUATION Expression COMMAND_END { code.push_back(new Node(EQUATION, $1, $3)); }
| Variable OP_EQUATION Expression COMMAND_END { code.push_back(new Node(EQUATION, $1, $3)); }
;

Expression: Expression OP_PLUS Expression { $$ = new Node(PLUS, $1, $3); }
| Expression OP_MINUS Expression { $$ = new Node(MINUS, $1, $3); }
| PRINT LEFT_BRACKET Expression RIGHT_BRACKET { $$ = new Node(PRINT_F, $3); }
| INPUT LEFT_BRACKET Expression RIGHT_BRACKET { $$ = new Node(INPUT_F, $3); }
| Variable { $$ = $1; }
| Literal {$$ = $1; }
;

Declaration: VAR_D Variable { $$ = new Node(VAR_DECL, $2); }
| CONST_D Variable { $$ = new Node(CONST_DECL, $2); }
;

Variable: VAR { $$ = new Node(*$1, VAR_NAME); }
;

Literal: INT_L { $$ = new Node(*$1); }
| FLOAT_L { $$ = new Node(*$1); }
| STRING_L { $$ = new Node(*$1); }
;

%%