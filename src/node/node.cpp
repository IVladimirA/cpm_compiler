#include "node.h"

Node::Node(std::string string, OpType operation) {
    value = string;
    left = nullptr;
    right = nullptr;
    op = operation;
}

Node::Node(OpType operation, Node* l, Node* r) {
    op = operation;
    left = l;
    right = r;
    value = "";
}

std::string Node::generateLine() {
    std::string result;
    switch (op) {
        case PLUS:
            result = left->generateLine() + " + " + right->generateLine();
            break;
        case MINUS:
            result = left->generateLine() + " - " + right->generateLine();
            break;
        case EQUATION:
            result = left->generateLine() + " = " + right->generateLine();
            break;
        case CONST_DECL:
            result = "const Mixed " + left->generateLine();
            break;
        case VAR_DECL:
            result = "Mixed " + left->generateLine();
            break;
        case PRINT_F:
            result = "print(" + left->generateLine() + ")";
            break;
        case INPUT_F:
            result = "input(" + left->generateLine() + ")";
            break;
        case VAR_NAME:
            result = value;
            break;
        case LIT:
            result = "Mixed(" + value + ")";
            break;
        default:
            result = "ERROR";
            break;
    }
    return result;
}

int Node::check_line(std::unordered_set<std::string>& consts, std::unordered_set<std::string>& vars_defined, std::unordered_set<std::string>& vars_declared, std::array<int, 4>& errors) {
    int result = 0;
    switch (op) {
        case PLUS:
        case MINUS:
            result = std::max(result, left->check_line(consts, vars_defined, vars_declared, errors));
            result = std::max(result, right->check_line(consts, vars_defined, vars_declared, errors));
            break;
        case EQUATION:
            result = std::max(result, right->check_line(consts, vars_defined, vars_declared, errors));
            if (left->op == VAR_NAME && consts.find(left->value) != consts.end()) {
                ++errors[3];
                result = 1;
            } else if (left->op == VAR_NAME)
                vars_defined.insert(left->value);
            result = std::max(result, left->check_line(consts, vars_defined, vars_declared, errors));
            break;
        case CONST_DECL:
            if (consts.find(left->value) != consts.end()) {
                ++errors[0];
                result = 1;
            } else if (vars_declared.find(left->value) != vars_declared.end()) {
                ++errors[1];
                result = 1;
            } else {
                consts.insert(left->value);
            }
            break;
        case VAR_DECL:
            if (consts.find(left->value) != consts.end()) {
                ++errors[0];
                result = 1;
            } else if (vars_declared.find(left->value) != vars_declared.end()) {
                ++errors[1];
                result = 1;
            } else {
                vars_declared.insert(left->value);
            }
            break;
        case PRINT_F:
        case INPUT_F:
            result = std::max(result, left->check_line(consts, vars_defined, vars_declared, errors));
            break;
        case VAR_NAME:
            if (consts.find(value) == consts.end() && vars_declared.find(value) == vars_declared.end()) {
                ++errors[2];
                result = 1;
            }
            break;
        case LIT:
        default:
            break;
    }
    return result;
}