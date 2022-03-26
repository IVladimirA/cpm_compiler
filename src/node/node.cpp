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

std::string Node::getStr() {
    std::string result;
    switch (op) {
        case PLUS:
            result = left->getStr() + " + " + right->getStr();
            break;
        case MINUS:
            result = left->getStr() + " - " + right->getStr();
            break;
        case EQUATION:
            result = left->getStr() + " = " + right->getStr();
            break;
        case CONST_DECL:
            result = "val " + left->getStr();
            break;
        case VAR_DECL:
            result = "var " + left->getStr();
            break;
        case PRINT_F:
            result = "print(" + left->getStr() + ")";
            break;
        case INPUT_F:
            result = "input(" + left->getStr() + ")";
            break;
        case VAR_NAME:
        case LIT:
            result = value;
            break;
        default:
            result = "ERROR";
            break;
    }
    return result;
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
        case LIT:
            result = value;
            break;
        default:
            result = "ERROR";
            break;
    }
    return result;
}