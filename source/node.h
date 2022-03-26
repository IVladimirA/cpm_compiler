#ifndef NODE_H
#define NODE_H

#include <string>
#include "mixed.h"

enum OpType {
    PLUS, MINUS, DECL, EQUATION,
    PRINT_F, INPUT_F,
    VAR_NAME, LIT
};

class Node {
    Node* left;
    Node* right;
    Mixed value;
    OpType op;
    public:
        Node();
        Node(std::string str, OpType o = LIT, bool is_const = false);
        Node(int num, OpType o = LIT, bool is_const = false);
        Node(long long int num, OpType o = LIT, bool is_const = false);
        Node(double num, OpType o = LIT, bool is_const = false);
        Node(OpType o, Node* l, Node* r = nullptr);
        std::string getStr();
        bool isEqual(Node* tree);
};

#endif