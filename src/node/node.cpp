#include "node.h"
#include "../visitor/visitor.h"

// Все слиплось, выглядит не очень.

// Как по мне эту строку можно записать красивее:
//   Literal::Literal(const std::string& val) : value(val) {}
Literal::Literal(const std::string& val)
    : value{val} { // Хм, а почему там фигурные скобки, а не круглые?

}
std::string Literal::get_value() const {
    return value;
}
void Literal::accept(Visitor& visitor) const {
    visitor.visit(this);
}
NodeType Literal::get_node_type() const {
    return node_literal;
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
NodeType Identifier::get_node_type() const {
    return node_identifier;
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
NodeType Comment::get_node_type() const {
    return node_comment;
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
NodeType Statement::get_node_type() const {
    return node_statement;
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
    delete identifier; // Delete called on 'Node' that is abstract but has non-virtual destructor
}
NodeType Declaration::get_node_type() const {
    return node_declaration;
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
    // Вот тут ты кажется накосячил с паттерном Visitor.
    // Логика здесь должна описывать логику обхода этого узла.
    // В текущем варианте здесь этого нет.
    // Из-за этого в visitor.cpp в функции void CodeChecker::visit(const BinaryOperation* bin_op)
    // У тебя явные вызовы:
    //   visit(bin_op->get_right());
    //   visit(bin_op->get_left());
    //
    // А должно быть тут:
    //   visitor.visit(this);
    //
    //   left->accept(visitor);
    //   right->accept(visitor);
    //
    // Но в идеале тогда visit() должен возвращать bool,
    // чтобы можно было останавливать процесс обхода, если
    // это не требуется или обход этой части дерева был
    // уже совершен вручную и повторный обход нежелателен.
    //
    //   if (!visitor.visit(this)) {
    //     return;
    //   }
    //
    //   left->accept(visitor);
    //   right->accept(visitor);
   visitor.visit(this);
}
BinaryOperation::~BinaryOperation() {
    delete left;
    delete right;
}
NodeType BinaryOperation::get_node_type() const {
    return node_bin_op;
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

NodeType UnaryArgFunction::get_node_type() const {
    return node_un_arg_func;
}