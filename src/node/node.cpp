#include "node.h"

Node::Node(const std::string& string, OpType operation) 
    : left{nullptr}, right{nullptr}, value {string}, op {operation} {
}

Node::Node(OpType operation, const Node* l, const Node* r, const std::string& val) 
    : left{l}, right{r}, value {val}, op {operation} {
}

std::string Node::generate_command() const {
    std::string result;
    switch (op) {
        case COMMAND:
            result = "\n" + left->generate_command() + ";";
            break;
        case PLUS:
            result = left->generate_command() + " + " + right->generate_command();
            break;
        case MINUS:
            result = left->generate_command() + " - " + right->generate_command();
            break;
        case EQUATION:
            result = left->generate_command() + " = " + right->generate_command();
            break;
        case CONST_DECL:
            result = "const Mixed " + left->generate_command();
            break;
        case VAR_DECL:
            result = "Mixed " + left->generate_command();
            break;
        case PRINT_F:
            result = "print(" + left->generate_command() + ")";
            break;
        case INPUT_F:
            result = "input(" + left->generate_command() + ")";
            break;
        case VAR_NAME:
        case COMM:
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

bool Node::check_command(std::unordered_set<std::string>& consts, std::unordered_set<std::string>& vars_defined, std::unordered_set<std::string>& vars_declared, std::array<int, 4>& errors) const {
    bool error = false;
    switch (op) {
        case PLUS:
        case MINUS:
            error |= left->check_command(consts, vars_defined, vars_declared, errors);
            error |= right->check_command(consts, vars_defined, vars_declared, errors);
            break;
        case EQUATION:
            error |= right->check_command(consts, vars_defined, vars_declared, errors);
            if (left->op == VAR_NAME && consts.find(left->value) != consts.end()) {
                ++errors[3];
                error = true;
            } else if (left->op == VAR_NAME)
                vars_defined.insert(left->value);
            error |= left->check_command(consts, vars_defined, vars_declared, errors);
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
        case COMMAND:
        case PRINT_F:
        case INPUT_F:
            error |= left->check_command(consts, vars_defined, vars_declared, errors);
            break;
        case VAR_NAME:
            if (consts.find(value) == consts.end() && vars_declared.find(value) == vars_declared.end()) {
                ++errors[2];
                error = true;
            }
            break;
        case LIT:
        case COMM:
            break;
        default:
            throw std::logic_error("Incorrect op of Node");;
    }
    return error;
}

Node::~Node() {
    delete left;
    delete right;
}