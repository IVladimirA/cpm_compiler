#include "nonterminals.h"
#include "../visitor/visitor.h"

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
    : type(tp), arguments(std::move(args)) {}

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


Root::Root(std::vector<const Node*> blocks) : code_blocks(std::move(blocks)) {}

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
