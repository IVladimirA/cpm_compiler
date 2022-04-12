#include <vector>
#include <iostream>
#include "node.h"
#include "../visitor/visitor.h"

Literal::Literal(const std::string& val)
    : value(val) {

}
void Literal::accept(Visitor& visitor) const {
    visitor.visit(this);
}
const Node* Literal::get_last() const {
    return nullptr;
}


Identifier::Identifier(const std::string& val)
    : name(val) {

}
void Identifier::accept(Visitor& visitor) const {
    visitor.visit(this);
}
const Node* Identifier::get_last() const {
    return nullptr;
}


Comment::Comment(const std::string& info)
    : information{info} {

}
void Comment::accept(Visitor& visitor) const {
    visitor.visit(this);
}
const Node* Comment::get_last() const {
    return nullptr;
}


Statement::Statement(const Node* comm)
    : command(comm) {

}
void Statement::accept(Visitor& visitor) const {
    if (!visitor.visit(this)) {
        return;
    }
    command->accept(visitor);
}
const Node* Statement::get_last() const {
    return command;
}
Statement::~Statement() {
    delete command;
}


Declaration::Declaration(DeclarationType t, const Node* id)
    : type(t), identifier(id) {

}
void Declaration::accept(Visitor& visitor) const {
    if (!visitor.visit(this)) {
        return;
    }
    identifier->accept(visitor);
}
const Node* Declaration::get_last() const {
    return identifier;
}
Declaration::~Declaration() {
    delete identifier;
}


BinaryOperation::BinaryOperation(BinOpType op, const Node* l, const Node* r)
    : type(op), left(l), right(r) {

}
void BinaryOperation::accept(Visitor& visitor) const {
    if (!visitor.visit(this)) {
        return;
    }
    left->accept(visitor);
    right->accept(visitor);
}
const Node* BinaryOperation::get_last() const {
    return right;
}
BinaryOperation::~BinaryOperation() {
    delete left;
    delete right;
}


FunctionCall::FunctionCall(FunctionType t, std::vector<const Node*> args)
    : type(t), arguments(args) {

}
void FunctionCall::accept(Visitor& visitor) const {
    if (!visitor.visit(this)) {
        return;
    }
    for (const Node* arg : arguments) {
        arg->accept(visitor);
    }
}
const Node* FunctionCall::get_last() const {
    return arguments.back();
}
FunctionCall::~FunctionCall() {
    for (const Node* tree : arguments) {
        delete tree;
    }
}
