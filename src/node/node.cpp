#include <vector>
// Неиспользуемый include.
#include <iostream>
#include "node.h"
#include "../visitor/visitor.h"

// Вот тут все очень очень некрасиво слиплось в кучу.
// Clang-Tidy: Pass by value and use std::move
// https://stackoverflow.com/questions/51705967/advantages-of-pass-by-value-and-stdmove-over-pass-by-reference
Literal::Literal(const std::string& val) : value(val) {}
void Literal::accept(Visitor& visitor) const {
    visitor.visit(this);
}
const Node* Literal::get_last() const {
    return nullptr;
}
// А вот тут аж три строки пустых. Почему просто не разделить одни определения
// одной строкой, а блоки двумя?


Identifier::Identifier(const std::string& val) : name(val) {}
void Identifier::accept(Visitor& visitor) const {
    visitor.visit(this);
}
const Node* Identifier::get_last() const {
    return nullptr;
}



Comment::Comment(const std::string& info) : information{info} {}
void Comment::accept(Visitor& visitor) const {
    visitor.visit(this);
}
const Node* Comment::get_last() const {
    return nullptr;
}



Statement::Statement(const Node* comm) : command(comm) {}
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



Declaration::Declaration(DeclarationType tp, const Node* id)
    : type(tp), identifier(id) {}
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



BinaryOperation::BinaryOperation(BinOpType op, const Node* l_arg, const Node* r_arg)
    : type(op), left(l_arg), right(r_arg) {}
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



FunctionCall::FunctionCall(FunctionType tp, std::vector<const Node*> args)
    : type(tp), arguments(args) {}
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
    for (const Node* arg : arguments) {
        delete arg;
    }
}



Root::Root() {} // Clang-Tidy: Use '= default' to define a trivial default constructor
// А вот тут ты забыл переместить blocks с помощью std::move, из-за этого лишнее копирование.
// Clang-Tidy: Parameter 'blocks' is passed by value and only copied once; consider moving it to avoid unnecessary copies
Root::Root(std::vector<const Node*> blocks) : code_blocks(blocks) {}
void Root::accept(Visitor& visitor) const {
    if (!visitor.visit(this)) {
        return;
    }
    for (const Node* block : code_blocks) {
        block->accept(visitor);
    }
}
const Node* Root::get_last() const {
    return code_blocks.back();
}
Root::~Root() {
    for (const Node* block : code_blocks) {
        delete block;
    }
}