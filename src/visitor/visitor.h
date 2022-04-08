#ifndef CPM_COMPILER_VISITOR_VISITOR_H_
#define CPM_COMPILER_VISITOR_VISITOR_H_

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
    virtual void visit(const Node* tree) = 0;
    virtual void visit(const Literal* lit) = 0;
    virtual void visit(const Identifier* id) = 0;
    virtual void visit(const Comment* comm) = 0;
    virtual void visit(const Statement* st) = 0;
    virtual void visit(const Declaration* decl) = 0;
    virtual void visit(const BinaryOperation* op) = 0;
    virtual void visit(const UnaryArgFunction* f) = 0;
};

class CodeGenerator : public Visitor {
public:
    CodeGenerator(std::string* st);
    void update_string(std::string* st);
    void visit(const Node* tree) override;
    void visit(const Literal* lit) override;
    void visit(const Identifier* id) override;
    void visit(const Comment* comm) override;
    void visit(const Statement* st) override;
    void visit(const Declaration* decl) override;
    void visit(const BinaryOperation* op) override;
    void visit(const UnaryArgFunction* func) override;
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
    void visit(const Node* tree) override;
    void visit(const Literal* lit) override;
    void visit(const Identifier* id) override;
    void visit(const Comment* comm) override;
    void visit(const Statement* st) override;
    void visit(const Declaration* decl) override;
    void visit(const BinaryOperation* op) override;
    void visit(const UnaryArgFunction* func) override;
private:
    std::vector<const Error*>* errors;
    std::unordered_set<std::string>* consts;
    std::unordered_set<std::string>* vars_declared;
    std::unordered_set<std::string>* vars_defined;
};

#endif