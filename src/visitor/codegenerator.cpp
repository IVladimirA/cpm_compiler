#include "codegenerator.h"
#include "../node/terminals.h"
#include "../node/nonterminals.h"

std::string CodeGenerator::get_code() {
    return code;
}

bool CodeGenerator::clear() {
    code.clear();
    seen.clear();
    return true;
}

void CodeGenerator::compute_last(const Node* tree) {
    if (tree->cast<Statement>()) {
        compute_last(tree->cast<Statement>());
        return;
    }
    if (tree->cast<Declaration>()) {
        compute_last(tree->cast<Declaration>());
        return;
    }
    if (tree->cast<BinaryOperation>()) {
        compute_last(tree->cast<BinaryOperation>());
        return;
    }
    if (tree->cast<FunctionCall>()) {
        compute_last(tree->cast<FunctionCall>());
        return;
    }
    if (tree->cast<Root>()) {
        compute_last(tree->cast<Root>());
        return;
    }
    throw std::invalid_argument("Unknown Node type");
}

bool CodeGenerator::visit(const Literal* lit) {
    seen.back().second.push_back("Mixed(" + lit->value + ")");
    if (!seen.empty() && seen.back().first->get_last()->cast<Literal>() == lit) {
        compute_last(seen.back().first);
    }
    return true;
}

bool CodeGenerator::visit(const Identifier* id) {
    seen.back().second.push_back(id->name);
    if (!seen.empty() && seen.back().first->get_last()->cast<Identifier>() == id) {
        compute_last(seen.back().first);
    }
    return true;
}

bool CodeGenerator::visit(const Comment* comm) {
    seen.back().second.push_back(comm->information);
    if (!seen.empty() && seen.back().first->get_last()->cast<Comment>() == comm) {
        compute_last(seen.back().first);
    }
    return true;
}

bool CodeGenerator::visit(const Statement* st) {
    seen.push_back({st, {}});
    return true;
}

void CodeGenerator::compute_last(const Statement* st) {
    std::vector<std::string> child_code = seen.back().second;
    seen.pop_back();
    seen.back().second.push_back("\n" + child_code[0] + ";");
    if (!seen.empty() && seen.back().first->get_last()->cast<Statement>() == st) {
        compute_last(seen.back().first);
    }
}

bool CodeGenerator::visit(const Declaration* decl) {
    seen.push_back({decl, {}});
    return true;
}

void CodeGenerator::compute_last(const Declaration* decl) {
    std::vector<std::string> child_code = seen.back().second;
    seen.pop_back();
    switch (decl->type) {
        case DeclarationType::CONST:
            seen.back().second.push_back("const Mixed " + child_code[0]);
            break;
        case DeclarationType::VAR:
            seen.back().second.push_back("Mixed " + child_code[0]);
            break;
        default:
            throw std::invalid_argument("Unknown DeclarationType");
    }
    if (!seen.empty() && seen.back().first->get_last()->cast<Declaration>() == decl) {
        compute_last(seen.back().first);
    }
}

bool CodeGenerator::visit(const BinaryOperation* bin_op) {
    seen.push_back({bin_op, {}});
    return true;
}

void CodeGenerator::compute_last(const BinaryOperation* bin_op) {
    std::vector<std::string> child_code = seen.back().second;
    seen.pop_back();
    switch (bin_op->type) {
        case BinOpType::ADDITION:
            seen.back().second.push_back(child_code[0] + " + " + child_code[1]);
            break;
        case BinOpType::SUBTRACTION:
            seen.back().second.push_back(child_code[0] + " - " + child_code[1]);
            break;
        case BinOpType::ASSIGNMENT:
            seen.back().second.push_back(child_code[0] + " = " + child_code[1]);
            break;
        default:
            throw std::invalid_argument("Unknown BinOpType");
    }
    if (!seen.empty() && seen.back().first->get_last()->cast<BinaryOperation>() == bin_op) {
        compute_last(seen.back().first);
    }
}

bool CodeGenerator::visit(const FunctionCall* func) {
    seen.push_back({func, {}});
    return true;
}

void CodeGenerator::compute_last(const FunctionCall* func) {
    std::vector<std::string> child_code = seen.back().second;
    seen.pop_back();
    switch (func->type) {
        case FunctionType::PRINT:
            seen.back().second.push_back("print(" + child_code[0] + ")");
            break;
        case FunctionType::INPUT:
            seen.back().second.push_back("input(" + child_code[0] + ")");
            break;
        default:
            throw std::invalid_argument("Unknown FunctionType");
    }
    if (!seen.empty() && seen.back().first->get_last()->cast<FunctionCall>() == func) {
        compute_last(seen.back().first);
    }
}

bool CodeGenerator::visit(const Root* tree) {
    seen.push_back({tree, {}});
    return true;
}

void CodeGenerator::compute_last(const Root* tree) {
    std::vector<std::string> child_code = seen.back().second;
    seen.pop_back();
    for (const std::string& block : child_code) {
        code += block;
    }
}
