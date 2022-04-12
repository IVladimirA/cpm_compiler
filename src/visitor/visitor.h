#pragma once

#include <string>
#include <vector>
#include <unordered_set>

class Node;
class Literal;
class Identifier;
class Comment;
class Statement;
class Declaration;
class BinaryOperation;
class FunctionCall;
class Error;

class Visitor {
public:
    virtual bool visit(const Node* tree) = 0;
    virtual bool visit(const Literal* lit) = 0;
    virtual bool visit(const Identifier* id) = 0;
    virtual bool visit(const Comment* comm) = 0;
    virtual bool visit(const Statement* st) = 0;
    virtual bool visit(const Declaration* decl) = 0;
    virtual bool visit(const BinaryOperation* op) = 0;
    virtual bool visit(const FunctionCall* f) = 0;
    virtual ~Visitor() = default;
};

class CodeGenerator : public Visitor {
public:
    CodeGenerator();
    std::string get_code();
    bool clear();
    bool visit(const Node* tree) override;
    void check_last(const Node* tree);
    bool visit(const Literal* lit) override;
    void check_last(const Literal* lit);
    bool visit(const Identifier* id) override;
    void check_last(const Identifier* id);
    bool visit(const Comment* comm) override;
    void check_last(const Comment* comm);
    bool visit(const Statement* st) override;
    void check_last(const Statement* st);
    bool visit(const Declaration* decl) override;
    void check_last(const Declaration* decl);
    bool visit(const BinaryOperation* op) override;
    void check_last(const BinaryOperation* op);
    bool visit(const FunctionCall* func) override;
    void check_last(const FunctionCall* func);
private:
    std::vector<std::pair<const Node*, std::vector<std::string>>> seen = {};
    std::string code = "";
};

class CodeChecker : public Visitor {
public:
    CodeChecker(
        std::vector<const Error*>* err,
        std::unordered_set<std::string>* con,
        std::unordered_set<std::string>* vars_decl,
        std::unordered_set<std::string>* vars_def);
    void update(
        std::vector<const Error*>* err,
        std::unordered_set<std::string>* con,
        std::unordered_set<std::string>* vars_decl,
        std::unordered_set<std::string>* vars_def);
    bool visit(const Node* tree) override;
    bool visit(const Literal* lit) override;
    bool visit(const Identifier* id) override;
    bool visit(const Comment* comm) override;
    bool visit(const Statement* st) override;
    bool visit(const Declaration* decl) override;
    bool visit(const BinaryOperation* op) override;
    bool visit(const FunctionCall* func) override;
private:
    std::vector<const Error*>* errors;
    std::unordered_set<std::string>* consts;
    std::unordered_set<std::string>* vars_declared;
    std::unordered_set<std::string>* vars_defined;
};

