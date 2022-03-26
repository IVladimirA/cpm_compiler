#ifndef NODE_H
#define NODE_H

#include <string>

enum OpType {
    PLUS, MINUS, CONST_DECL, VAR_DECL, EQUATION,
    PRINT_F, INPUT_F,
    VAR_NAME, LIT
};

class Node {
public:
    Node(std::string string, OpType operation = LIT);
    Node(OpType operation, Node* l, Node* r = nullptr);
    std::string getStr();
    std::string generateLine();
    bool isEqual(Node* tree);
private:
    Node* left;
    Node* right;
    std::string value;
    OpType op;
};

#endif