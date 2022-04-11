#include <vector>
#include "node.h"
#include "../visitor/visitor.h"

Literal::Literal(const std::string& val)
    : value(val) {

}
void Literal::accept(Visitor& visitor) const {
    visitor.visit(this);
}


Identifier::Identifier(const std::string& val)
    : name(val) {

}
void Identifier::accept(Visitor& visitor) const {
    visitor.visit(this);
}


Comment::Comment(const std::string& info)
    : information{info} {

}
void Comment::accept(Visitor& visitor) const {
    visitor.visit(this);
}


Statement::Statement(const Node* comm)
    : command(comm) {

}
void Statement::accept(Visitor& visitor) const {
    visitor.visit(this);
}
Statement::~Statement() {
    delete command;
}


Declaration::Declaration(DeclarationType t, const Node* id)
    : type(t), identifier(id) {

}
void Declaration::accept(Visitor& visitor) const {
    visitor.visit(this);
}
Declaration::~Declaration() {
    delete identifier;
}


BinaryOperation::BinaryOperation(BinOpType op, const Node* l, const Node* r)
    : type(op), left(l), right(r) {

}
void BinaryOperation::accept(Visitor& visitor) const {
    visitor.visit(this);
}
BinaryOperation::~BinaryOperation() {
    delete left;
    delete right;
}


FunctionCall::FunctionCall(FunctionType t, std::vector<const Node*> args)
    : type(t), arguments(args) {

}
void FunctionCall::accept(Visitor& visitor) const {
    visitor.visit(this);
}
FunctionCall::~FunctionCall() {
    for (const Node* tree : arguments) {
        delete tree;
    }
}
