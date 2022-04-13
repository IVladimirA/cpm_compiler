#include "terminals.h"
#include "../visitor/visitor.h"

Literal::Literal(std::string val) : value(std::move(val)) {}

void Literal::accept(Visitor& visitor) const {
    visitor.visit(this);
}

const Node* Literal::get_last() const {
    return nullptr;
}


Identifier::Identifier(std::string val) : name(std::move(val)) {}

void Identifier::accept(Visitor& visitor) const {
    visitor.visit(this);
}

const Node* Identifier::get_last() const {
    return nullptr;
}


Comment::Comment(std::string info) : information(std::move(info)) {}

void Comment::accept(Visitor& visitor) const {
    visitor.visit(this);
}

const Node* Comment::get_last() const {
    return nullptr;
}
