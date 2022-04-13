#pragma once

#include "../visitor/visitor.h"

class CodeGenerator : public Visitor {
public:
    CodeGenerator() = default;
    std::string get_code();
    bool clear();
    bool visit(const Node* tree) override;
    void compute_last(const Node* tree);
    bool visit(const Literal* lit) override;
    bool visit(const Identifier* id) override;
    bool visit(const Comment* comm) override;
    bool visit(const Statement* st) override;
    void compute_last(const Statement* st);
    bool visit(const Declaration* decl) override;
    void compute_last(const Declaration* decl);
    bool visit(const BinaryOperation* op) override;
    void compute_last(const BinaryOperation* op);
    bool visit(const FunctionCall* func) override;
    void compute_last(const FunctionCall* func);
    bool visit(const Root* tree) override;
    void compute_last(const Root* tree);
private:
    std::vector<std::pair<const Node*, std::vector<std::string>>> seen = {};
    std::string code = "";
};
