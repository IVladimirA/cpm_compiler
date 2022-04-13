#include "codechecker.h"
#include "../node/terminals.h"
#include "../node/nonterminals.h"


void CodeChecker::clear_errors() {
    errors.clear();
}

void CodeChecker::clear_seen() {
    consts.clear();
    vars_declared.clear();
    vars_defined.clear();
}

void CodeChecker::clear() {
    clear_errors();
    clear_seen();
}

void CodeChecker::error(std::string info) {
    errors.push_back(std::move(info));
}

bool CodeChecker::visit(const Node* tree) {
    return Visitor::visit(tree);
}

bool CodeChecker::visit(const Literal* lit) {
    return false;
}

bool CodeChecker::visit(const Identifier* id) {
    const bool is_defined_const = consts.find(id->name) != consts.end();
    const bool is_defined_var = vars_defined.find(id->name) != vars_defined.end();
    if (!is_defined_const && !is_defined_var) {
        error("usage of undefined identifier \'" + id->name + "\'");
    }
    return false;
}

bool CodeChecker::visit(const Comment* comm) {
    return false;
}

bool CodeChecker::visit(const Statement* st) {
    return true;
}

bool CodeChecker::visit(const Declaration* decl) {
    auto id = decl->identifier->cast<Identifier>();
    if (consts.find(id->name) != consts.end()) {
        error("redeclaration of constant \'" + id->name + "\'");
        return false;
    }
    if (vars_declared.find(id->name) != vars_declared.end()) {
        error("redeclaration of variable \'" + id->name + "\'");
        return false;
    }
    switch(decl->type) {
        case DeclarationType::CONST:
            consts.insert(id->name);
            return false;
        case DeclarationType::VAR:
            vars_declared.insert(id->name);
            return false;
        default:
            throw std::invalid_argument("Unknown DeclarationType");
    }
    return false;
}

bool CodeChecker::visit(const BinaryOperation* bin_op) {
    switch(bin_op->type) {
        case BinOpType::ADDITION:
        case BinOpType::SUBTRACTION:
            return true;
        case BinOpType::ASSIGNMENT:
            if (bin_op->left->cast<Identifier>()) {
                auto id = bin_op->left->cast<Identifier>();
                if (consts.find(id->name) != consts.end()) {
                    error("redefinition of constant \'" + id->name + "\'");
                    return true;
                }
                if (vars_declared.find(id->name) != vars_declared.end()) {
                    vars_defined.insert(id->name);
                    return true;
                }
            }
            if (bin_op->left->cast<Declaration>()) {
                auto decl = bin_op->left->cast<Declaration>();
                if (decl->type == DeclarationType::VAR) {
                    vars_defined.insert(decl->identifier->cast<Identifier>()->name);
                }
                return true;
            }
            return true;
        default:
            throw std::invalid_argument("Unknown BinOpType");
    }
    return true;
}

bool CodeChecker::visit(const FunctionCall* func) {
    switch(func->type) {
        case FunctionType::PRINT:
        case FunctionType::INPUT:
            return true;
        default:
            throw std::invalid_argument("Unknown FunctionType");
    }
    return true;
}

bool CodeChecker::visit(const Root* tree) {
    return true;
}
