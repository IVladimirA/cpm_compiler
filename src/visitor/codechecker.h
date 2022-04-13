#pragma once

#include "../visitor/visitor.h"

class CodeChecker : public Visitor {
public:
    CodeChecker() = default;
    void clear_errors();
    void clear_seen();
    void clear();
    void error(std::string info);
    bool visit(const Literal* lit) override;
    bool visit(const Identifier* id) override;
    bool visit(const Comment* comm) override;
    bool visit(const Statement* st) override;
    bool visit(const Declaration* decl) override;
    bool visit(const BinaryOperation* op) override;
    bool visit(const FunctionCall* func) override;
    bool visit(const Root* tree) override;
    std::vector<std::string> errors;
private:
    std::unordered_set<std::string> consts;
    std::unordered_set<std::string> vars_declared;
    std::unordered_set<std::string> vars_defined;
};
