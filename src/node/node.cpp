#include "node.h"

Node::Node(const std::string& string, OpType operation) 
    : left{nullptr}, right{nullptr}, value {string}, op {operation} {
}

Node::Node(OpType operation, const Node* l, const Node* r, const std::string& val) 
    : left{l}, right{r}, value {val}, op {operation} {
}

std::string Node::generate_command() const {
    switch (op) {
        case COMMAND:
            return "\n" + left->generate_command() + ";";
        case PLUS:
            return left->generate_command() + " + " + right->generate_command();
        case MINUS:
            return left->generate_command() + " - " + right->generate_command();
        case EQUATION:
            return left->generate_command() + " = " + right->generate_command();
        case CONST_DECL:
            return "const Mixed " + left->generate_command();
        case VAR_DECL:
            return "Mixed " + left->generate_command();
        case PRINT_F:
            return "print(" + left->generate_command() + ")";
        case INPUT_F:
            return "input(" + left->generate_command() + ")";
        case LIT:
            return "Mixed(" + value + ")";
        case VAR_NAME:
        case COMM:
        default:
            return value;
    }
}

bool Node::check_command(std::unordered_set<std::string>& consts, std::unordered_set<std::string>& vars_defined, std::unordered_set<std::string>& vars_declared, std::array<int, 4>& errors) const {
    switch (op) {
        case PLUS:
        case MINUS:
            return left->check_command(consts, vars_defined, vars_declared, errors)
                || right->check_command(consts, vars_defined, vars_declared, errors);
        case EQUATION: {
            bool declaration_error = right->check_command(consts, vars_defined, vars_declared, errors);
            if (left->op == VAR_NAME && consts.find(left->value) != consts.end()) {
                ++errors[3];
                return true;
            }
            if (left->op == VAR_NAME)
                vars_defined.insert(left->value); // not all cases of definition covered
            return left->check_command(consts, vars_defined, vars_declared, errors)
                || declaration_error;
        }
        case CONST_DECL:
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
        case VAR_DECL:
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
        case COMMAND:
        case PRINT_F:
        case INPUT_F:
            return left->check_command(consts, vars_defined, vars_declared, errors);
        case VAR_NAME:
            if (consts.find(value) == consts.end() && vars_declared.find(value) == vars_declared.end()) {
                ++errors[2];
                return true;
            }
            return false;
        case LIT:
        case COMM:
        default:
            return false;
    }
}

Node::~Node() {
    delete left;
    delete right;
}