#include "node.h"

Node::Node() {
    value = Mixed();
    left = nullptr;
    right = nullptr;
    op = VAR_V;
}

Node::Node(std::string str, bool is_const) {
    value = Mixed(str, is_const);
    left = nullptr;
    right = nullptr;
    if (is_const)
        op = VAL_V;
    else
        op = VAR_V;
}

Node::Node(int num, bool is_const) {
    value = Mixed(num, is_const);
    left = nullptr;
    right = nullptr;
    if (is_const)
        op = VAL_V;
    else
        op = VAR_V;
}

Node::Node(long long int num, bool is_const) {
    value = Mixed(num, is_const);
    left = nullptr;
    right = nullptr;
    if (is_const)
        op = VAL_V;
    else
        op = VAR_V;
}

Node::Node(double num, bool is_const) {
    value = Mixed(num, is_const);
    left = nullptr;
    right = nullptr;
    if (is_const)
        op = VAL_V;
    else
        op = VAR_V;
}

Node::Node(OpType o, Node* l, Node* r) {
    op = o;
    left = l;
    right = r;
    value = Mixed();
}