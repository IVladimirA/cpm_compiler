%{

#include <iostream>
#include <string>
#include <vector>
#include "../node/node.h"

int yylex(void);
void yyerror(const char *error) {
    std::cerr << error;
}
std::vector<const Node*> code;

%}

%union {
    const Node* node;
    const std::string* name;
}

%token <name> VAR INT_L FLOAT_L STRING_L COMMENT
%token VAR_D CONST_D
%token OP_PLUS OP_MINUS OP_EQUATION
%token COMMAND_END
%token LEFT_BRACKET RIGHT_BRACKET
%token PRINT INPUT

%right OP_EQUATION
%left OP_PLUS
%left OP_MINUS

%type <node> Input Command Expression Function Declaration Variable Literal

%start Input

%%

Input: Command { code.push_back($1); }
| Input Command { code.push_back($2); }
;

Command: COMMENT { $$ = new Node(*$1, op_comment); }
| Expression COMMAND_END { $$ = new Node(op_command, $1); }
| Declaration OP_EQUATION Expression COMMAND_END { $$ = new Node(op_command, new Node(op_equation, $1, $3)); }
| Variable OP_EQUATION Expression COMMAND_END { $$ = new Node(op_command, new Node(op_equation, $1, $3)); }
;

Expression: Expression OP_PLUS Expression { $$ = new Node(op_plus, $1, $3); }
| Expression OP_MINUS Expression { $$ = new Node(op_minus, $1, $3); }
| Function { $$ = $1; }
| Variable { $$ = $1; }
| Literal {$$ = $1; }
;

Function: PRINT LEFT_BRACKET Expression RIGHT_BRACKET { $$ = new Node(op_print, $3); }
| INPUT LEFT_BRACKET Expression RIGHT_BRACKET { $$ = new Node(op_input, $3); }
;

Declaration: VAR_D Variable { $$ = new Node(op_var_decl, $2); }
| CONST_D Variable { $$ = new Node(op_const_decl, $2); }
;

Variable: VAR { $$ = new Node(*$1, op_variable); }
;

Literal: INT_L { $$ = new Node(*$1); }
| FLOAT_L { $$ = new Node(*$1); }
| STRING_L { $$ = new Node(*$1); }
;

%%