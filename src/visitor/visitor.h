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
class Root;

class Visitor {
public:
    virtual bool visit(const Literal* lit) = 0;
    virtual bool visit(const Identifier* id) = 0;
    virtual bool visit(const Comment* comm) = 0;
    virtual bool visit(const Statement* st) = 0;
    virtual bool visit(const Declaration* decl) = 0;
    virtual bool visit(const BinaryOperation* op) = 0;
    virtual bool visit(const FunctionCall* f) = 0;
    virtual bool visit(const Root* tree) = 0;
    virtual ~Visitor() = default;
};
