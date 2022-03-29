#ifndef CPM_COMPILER_NODE_NODE_H_
#define CPM_COMPILER_NODE_NODE_H_

#include <unordered_set>
#include <string>
#include <array>

enum OpType {
    PLUS, MINUS, CONST_DECL, VAR_DECL, EQUATION,
    PRINT_F, INPUT_F,
    VAR_NAME, LIT, COMM, COMMAND
};

class Node {
public:
    Node(const std::string& string, OpType operation = LIT);
    Node(OpType operation, const Node* l, const Node* r = nullptr, const std::string& val = "");
    std::string generate_command() const;
    bool check_command(std::unordered_set<std::string>& consts, std::unordered_set<std::string>& vars_defined, std::unordered_set<std::string>& vars_declared, std::array<int, 4>& errors) const;
    ~Node();
private:
    const Node* left;
    const Node* right;
    std::string value;
    OpType op;
};

#endif