#include "node.h"

Node::Node(const std::string& string, OperationType op) 
    : left{nullptr}, right{nullptr}, value{string}, operation{op} {
}

Node::Node(OperationType op, const Node* left_node, const Node* right_node, const std::string& val) 
    : left{left_node}, right{right_node}, value{val}, operation{op} {
}

// Generate cpp code of statement
std::string Node::generate_statement() const {
    switch (operation) {
        case op_statement:
            return "\n" + left->generate_statement() + ";";
        case op_addition:
            return left->generate_statement() + " + " + right->generate_statement();
        case op_subtraction:
            return left->generate_statement() + " - " + right->generate_statement();
        case op_equation:
            return left->generate_statement() + " = " + right->generate_statement();
        case op_const_decl:
            return "const Mixed " + left->generate_statement();
        case op_var_decl:
            return "Mixed " + left->generate_statement();
        case op_print:
            return "print(" + left->generate_statement() + ")";
        case op_input:
            return "input(" + left->generate_statement() + ")";
        case op_literal:
            return "Mixed(" + value + ")";
        case op_variable:
        case op_comment:
        default:
            return value;
    }
}

// Check presence of statement's errors
// Also update sets of seen constants and variables
bool Node::check_statement(std::unordered_set<std::string>& consts, std::unordered_set<std::string>& vars_defined,
    std::unordered_set<std::string>& vars_declared, std::array<int, 4>& errors) const {
    switch (operation) {
        case op_addition:
        case op_subtraction:
            return left->check_statement(consts, vars_defined, vars_declared, errors)
                || right->check_statement(consts, vars_defined, vars_declared, errors);
        case op_equation: {
            if (left->operation == op_variable && consts.find(left->value) != consts.end()) {
                ++errors[3];
                return true;
            }
            if (left->operation == op_variable && vars_declared.find(left->value) == vars_declared.end()) {
                ++errors[2];
                return true;
            }
            if (right->check_statement(consts, vars_defined, vars_declared, errors))
                return true;
            if (left->operation == op_variable) {
                vars_defined.insert(left->value);
                return false;
            }
            return left->check_statement(consts, vars_defined, vars_declared, errors);
        }
        case op_const_decl:
            if (consts.find(left->value) != consts.end()) {
                ++errors[0];
                return true;
            }
            if (vars_declared.find(left->value) != vars_declared.end()) {
                ++errors[1];
                return true;
            }
            consts.insert(left->value);
            return false;
        case op_var_decl:
            if (consts.find(left->value) != consts.end()) {
                ++errors[0];
                return true;
            }
            if (vars_declared.find(left->value) != vars_declared.end()) {
                ++errors[1];
                return true;
            }
            vars_declared.insert(left->value);
            return false;
        case op_statement:
        case op_print:
        case op_input:
            return left->check_statement(consts, vars_defined, vars_declared, errors);
        case op_variable:
            if (consts.find(value) == consts.end() && vars_declared.find(value) == vars_declared.end()) {
                ++errors[2];
                return true;
            }
            return false;
        case op_literal:
        case op_comment:
        default:
            return false;
    }
}

Node::~Node() {
    delete left;
    delete right;
}