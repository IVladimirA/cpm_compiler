#include "visitor.h"
#include "../node/terminals.h"
#include "../node/nonterminals.h"

bool Visitor::visit(const Node* tree) {
    if (tree->cast<Literal>()) {
        return visit(tree->cast<Literal>());
    }
    if (tree->cast<Identifier>()) {
        return visit(tree->cast<Identifier>());
    }
    if (tree->cast<Comment>()) {
        return visit(tree->cast<Comment>());
    }
    if (tree->cast<Statement>()) {
        return visit(tree->cast<Statement>());
    }
    if (tree->cast<Declaration>()) {
        return visit(tree->cast<Declaration>());
    }
    if (tree->cast<BinaryOperation>()) {
        return visit(tree->cast<BinaryOperation>());
    }
    if (tree->cast<FunctionCall>()) {
        return visit(tree->cast<FunctionCall>());
    }
    if (tree->cast<Root>()) {
        return visit(tree->cast<Root>());
    }
    throw std::invalid_argument("Unknown Node type");
}
