#ifndef CPM_COMPILER_NODE_NODE_H_
#define CPM_COMPILER_NODE_NODE_H_

#include <unordered_set>
#include "../error/error.h"

enum OperationType {
    op_statement,
    op_comment,
    op_addition,
    op_subtraction,
    op_const_decl,
    op_var_decl,
    op_assignment,
    op_print,
    op_input,
    op_variable,
    op_literal
};

class Node {
public:
    Node(const std::string& string, OperationType op = op_literal);
    Node(OperationType op, const Node* left_node, const Node* right_node = nullptr, const std::string& val = "");
    std::string generate_statement() const;
    bool check_statement(
        std::unordered_set<std::string>& consts,
        std::unordered_set<std::string>& vars_defined,
        std::unordered_set<std::string>& vars_declared,
        std::vector<const Error*>& errors) const;
    ~Node();
private:
    const Node* left;
    const Node* right;
    const std::string value;
    const OperationType operation;
};

#endif