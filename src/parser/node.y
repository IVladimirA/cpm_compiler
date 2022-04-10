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

// Чтобы ошибки парсинга быои подробными.
%define parse.error verbose

%union {
    const Node* node;
    const std::string* name;
}

%token <name> t_variable t_integer_literal t_float_literal t_string_literal t_comment
%token t_variable_declaration t_constant_declaration
%token t_plus t_minus t_equals
%token t_command_ending
%token t_left_bracket t_right_bracket
%token t_print t_input

%left t_plus t_minus

%type <node> Input Statement Command Expression Declaration Variable Literal

%start Input

%%

Input: Statement { code.push_back($1); }
| Input Statement { code.push_back($2); }
;

Statement: t_comment { $$ = new Comment(*$1); }
| Command t_command_ending { $$ = new Statement($1); }
;

Command: Expression { $$ = $1; }
| t_print t_left_bracket Expression t_right_bracket { $$ = new UnaryArgFunction(un_f_print, $3); }
| Declaration t_equals Expression { $$ = new BinaryOperation(assignment_op, $1, $3); }
| Variable t_equals Expression { $$ = new BinaryOperation(assignment_op, $1, $3); }
;

Expression: Expression t_plus Expression { $$ = new BinaryOperation(addition_op, $1, $3); }
| Expression t_minus Expression { $$ = new BinaryOperation(subtraction_op, $1, $3); }
| t_input t_left_bracket Expression t_right_bracket { $$ = new UnaryArgFunction(un_f_input, $3); }
| Variable { $$ = $1; }
| Literal { $$ = $1; }
;

Declaration: t_variable_declaration Variable { $$ = new Declaration(var_decl, $2); }
| t_constant_declaration Variable { $$ = new Declaration(const_decl, $2); }
;

Variable: t_variable { $$ = new Identifier(*$1); }
;

Literal: t_integer_literal { $$ = new Literal(*$1); }
| t_float_literal { $$ = new Literal(*$1); }
| t_string_literal { $$ = new Literal(*$1); }
;

%%