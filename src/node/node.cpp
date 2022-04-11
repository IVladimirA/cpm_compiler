#include "node.h"
#include "../visitor/visitor.h"

Literal::Literal(const std::string& val)
    : value{val} {

}
std::string Literal::get_value() const {
    return value;
}
void Literal::accept(Visitor& visitor) const {
    visitor.visit(this);
}


Identifier::Identifier(const std::string& val)
    : name{val} {

}
std::string Identifier::get_name() const {
    return name;
}
void Identifier::accept(Visitor& visitor) const {
    visitor.visit(this);
}


Comment::Comment(const std::string& info)
    : information{info} {

}
std::string Comment::get_information() const {
    return information;
}
void Comment::accept(Visitor& visitor) const {
    visitor.visit(this);
}


Statement::Statement(const Node* comm)
    : command{comm} {

}
const Node* Statement::get_command() const {
    return command;
}
void Statement::accept(Visitor& visitor) const {
    visitor.visit(this);
}
Statement::~Statement() {
    delete command;
}


Declaration::Declaration(DeclarationType t, const Node* id)
    : type{t}, identifier{id} {

}
const Node* Declaration::get_identifier() const {
    return identifier;
}
DeclarationType Declaration::get_type() const {
    return type;
}
void Declaration::accept(Visitor& visitor) const {
    visitor.visit(this);
}
Declaration::~Declaration() {
    delete identifier;
}


BinaryOperation::BinaryOperation(BinOpType op, const Node* l, const Node* r)
    : type(op), left{l}, right{r} {

}
const Node* BinaryOperation::get_left() const {
    return left;
}
const Node* BinaryOperation::get_right() const {
    return right;
}
BinOpType BinaryOperation::get_type() const {
    return type;
}
void BinaryOperation::accept(Visitor& visitor) const {
    visitor.visit(this);
}
BinaryOperation::~BinaryOperation() {
    delete left;
    delete right;
}


UnaryArgFunction::UnaryArgFunction(UnaryArgFuncType t, const Node* arg)
    : type(t), argument{arg} {

}
const Node* UnaryArgFunction::get_arg() const {
    return argument;
}
UnaryArgFuncType UnaryArgFunction::get_type() const {
    return type;
}
void UnaryArgFunction::accept(Visitor& visitor) const {
    visitor.visit(this);
}
UnaryArgFunction::~UnaryArgFunction() {
    delete argument;
}
