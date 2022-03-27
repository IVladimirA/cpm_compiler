#ifndef NODE_H
#define NODE_H

#include <unordered_set>
#include <string>
#include <array>

enum OpType {
    PLUS, MINUS, CONST_DECL, VAR_DECL, EQUATION,
    PRINT_F, INPUT_F,
    VAR_NAME, LIT
};

class Node {
public:
    Node(std::string string, OpType operation = LIT);
    Node(OpType operation, Node* l, Node* r = nullptr);
    std::string generateLine();
    bool isEqual(Node* tree);
    int check_line(std::unordered_set<std::string>& consts, std::unordered_set<std::string>& vars_defined, std::unordered_set<std::string>& vars_declared, std::array<int, 4>& errors);
    ~Node();
private:
    Node* left;
    Node* right;
    std::string value;
    OpType op;
};

#endif