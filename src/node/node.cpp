#include "node.h"

Node::Node(const std::string& string, OperationType op) 
    : left{nullptr}, right{nullptr}, value{string}, operation{op} {
}

Node::Node(OperationType op, const Node* left_node, const Node* right_node, const std::string& val) 
    : left{left_node}, right{right_node}, value{val}, operation{op} {
}

std::string Node::generate_command() const {
    switch (operation) {
        case op_command:
            return "\n" + left->generate_command() + ";";
        case op_plus:
            return left->generate_command() + " + " + right->generate_command();
        case op_minus:
            return left->generate_command() + " - " + right->generate_command();
        case op_equation:
            return left->generate_command() + " = " + right->generate_command();
        case op_const_decl:
            return "const Mixed " + left->generate_command();
        case op_var_decl:
            return "Mixed " + left->generate_command();
        case op_print:
            return "print(" + left->generate_command() + ")";
        case op_input:
            return "input(" + left->generate_command() + ")";
        case op_literal:
            return "Mixed(" + value + ")";
        case op_variable:
        case op_comment:
        default:
            return value;
    }
}

bool Node::check_command(std::unordered_set<std::string>& consts, std::unordered_set<std::string>& vars_defined,
    std::unordered_set<std::string>& vars_declared, std::array<int, 4>& errors) const {
    switch (operation) {
        case op_plus:
        case op_minus:
            return left->check_command(consts, vars_defined, vars_declared, errors)
                || right->check_command(consts, vars_defined, vars_declared, errors);
        case op_equation: {
            if (left->operation == op_variable && consts.find(left->value) != consts.end()) {
                ++errors[3];
                return true;
            }
            if (left->operation == op_variable && vars_declared.find(left->value) == vars_declared.end()) {
                ++errors[2];
                return true;
            }
            if (right->check_command(consts, vars_defined, vars_declared, errors))
                return true;
            if (left->operation == op_variable) {
                vars_defined.insert(left->value);
                return false;
            }
            return left->check_command(consts, vars_defined, vars_declared, errors);
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
        case op_command:
        case op_print:
        case op_input:
            return left->check_command(consts, vars_defined, vars_declared, errors);
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