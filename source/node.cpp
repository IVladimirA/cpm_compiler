#include "node.h"

Node::Node() {
    value = Mixed();
    left = nullptr;
    right = nullptr;
    op = VAR_NAME;
}

Node::Node(std::string str, OpType o, bool is_const) {
    value = Mixed(str, is_const);
    left = nullptr;
    right = nullptr;
    op = o;
}

Node::Node(int num, OpType o, bool is_const) {
    value = Mixed(num, is_const);
    left = nullptr;
    right = nullptr;
    op = o;
}

Node::Node(long long int num, OpType o, bool is_const) {
    value = Mixed(num, is_const);
    left = nullptr;
    right = nullptr;
    op = o;
}

Node::Node(double num, OpType o, bool is_const) {
    value = Mixed(num, is_const);
    left = nullptr;
    right = nullptr;
    op = o;
}

Node::Node(OpType o, Node* l, Node* r) {
    op = o;
    left = l;
    right = r;
    value = Mixed();
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
        case DECL:
            result = "decl " + left->getStr();
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