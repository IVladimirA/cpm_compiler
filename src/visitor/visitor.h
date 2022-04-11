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
class UnaryArgFunction;
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
    virtual bool visit(const UnaryArgFunction* f) = 0;
    virtual ~Visitor() = default;
};

class CodeGenerator : public Visitor {
public:
    CodeGenerator(std::string* st);
    void update_string(std::string* st);
    bool visit(const Node* tree) override;
    bool visit(const Literal* lit) override;
    bool visit(const Identifier* id) override;
    bool visit(const Comment* comm) override;
    bool visit(const Statement* st) override;
    bool visit(const Declaration* decl) override;
    bool visit(const BinaryOperation* op) override;
    bool visit(const UnaryArgFunction* func) override;
private:
    std::string* statement;
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
    bool visit(const UnaryArgFunction* func) override;
private:
    std::vector<const Error*>* errors;
    std::unordered_set<std::string>* consts;
    std::unordered_set<std::string>* vars_declared;
    std::unordered_set<std::string>* vars_defined;
};

