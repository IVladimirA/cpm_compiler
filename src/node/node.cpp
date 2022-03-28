#include "node.h"

Node::Node(std::string string, OpType operation) {
    value = string;
    left = nullptr;
    right = nullptr;
    op = operation;
}

Node::Node(OpType operation, Node* l, Node* r, std::string val) {
    op = operation;
    left = l;
    right = r;
    value = val;
}

std::string Node::generate_line() {
    std::string result;
    switch (op) {
        case COMM:
            result = left->generate_line() + " " + value;
            break;
        case COMMAND:
            result = left->generate_line() + ";";
            break;
        case PLUS:
            result = left->generate_line() + " + " + right->generate_line();
            break;
        case MINUS:
            result = left->generate_line() + " - " + right->generate_line();
            break;
        case EQUATION:
            result = left->generate_line() + " = " + right->generate_line();
            break;
        case CONST_DECL:
            result = "const Mixed " + left->generate_line();
            break;
        case VAR_DECL:
            result = "Mixed " + left->generate_line();
            break;
        case PRINT_F:
            result = "print(" + left->generate_line() + ")";
            break;
        case INPUT_F:
            result = "input(" + left->generate_line() + ")";
            break;
        case VAR_NAME:
            result = value;
            break;
        case LIT:
            result = "Mixed(" + value + ")";
            break;
        default:
            throw std::logic_error("Incorrect op of Node");
    }
    return result;
}

bool Node::check_line(std::unordered_set<std::string>& consts, std::unordered_set<std::string>& vars_defined, std::unordered_set<std::string>& vars_declared, std::array<int, 4>& errors) {
    bool error = false;
    switch (op) {
        case COMM:
        case COMMAND:
            error |= left->check_line(consts, vars_defined, vars_declared, errors);
            break;
        case PLUS:
        case MINUS:
            error |= left->check_line(consts, vars_defined, vars_declared, errors);
            error |= right->check_line(consts, vars_defined, vars_declared, errors);
            break;
        case EQUATION:
            error |= right->check_line(consts, vars_defined, vars_declared, errors);
            if (left->op == VAR_NAME && consts.find(left->value) != consts.end()) {
                ++errors[3];
                error = true;
            } else if (left->op == VAR_NAME)
                vars_defined.insert(left->value);
            error |= left->check_line(consts, vars_defined, vars_declared, errors);
            break;
        case CONST_DECL:
            if (consts.find(left->value) != consts.end()) {
                ++errors[0];
                error = true;
            } else if (vars_declared.find(left->value) != vars_declared.end()) {
                ++errors[1];
                error = true;
            } else {
                consts.insert(left->value);
            }
            break;
        case VAR_DECL:
            if (consts.find(left->value) != consts.end()) {
                ++errors[0];
                error = true;
            } else if (vars_declared.find(left->value) != vars_declared.end()) {
                ++errors[1];
                error = true;
            } else {
                vars_declared.insert(left->value);
            }
            break;
        case PRINT_F:
        case INPUT_F:
            error |= left->check_line(consts, vars_defined, vars_declared, errors);
            break;
        case VAR_NAME:
            if (consts.find(value) == consts.end() && vars_declared.find(value) == vars_declared.end()) {
                ++errors[2];
                error = true;
            }
            break;
        case LIT:
        default:
            break;
    }
    return error;
}

Node::~Node() {
    delete left;
    delete right;
}