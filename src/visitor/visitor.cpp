#include "visitor.h"
#include "../error/error.h"
#include "../node/node.h"


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

CodeGenerator::CodeGenerator() {}

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

bool CodeGenerator::visit(const Node* tree) {
    return Visitor::visit(tree);
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


CodeChecker::CodeChecker() {}

void CodeChecker::clear_errors() {
    for (const Error* e : errors)
        delete e;
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
CodeChecker::~CodeChecker() {
    clear();
}
bool CodeChecker::visit(const Node* tree) {
    return Visitor::visit(tree);
}

bool CodeChecker::visit(const Literal* lit) {
    return false;
}

bool CodeChecker::visit(const Identifier* id) {
    if (consts.find(id->name) == consts.end()
        && vars_defined.find(id->name) == vars_defined.end()) {
        errors.push_back(new UndefinedIdentifier({id->name}));
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
    const Identifier* id = decl->identifier->cast<Identifier>();
    if (consts.find(id->name) != consts.end()) {
        errors.push_back(new ConstantRedeclaration({id->name}));
        return false;
    }
    if (vars_declared.find(id->name) != vars_declared.end()) {
        errors.push_back(new VariableRedeclaration({id->name}));
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
            throw std::invalid_argument("Unknown declaration type");
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
                const Identifier* id = bin_op->left->cast<Identifier>();
                if (consts.find(id->name) != consts.end()) {
                    errors.push_back(new ConstantRedefinition({id->name}));
                    return true;
                }
                if (vars_declared.find(id->name) != vars_declared.end()) {
                    vars_defined.insert(id->name);
                    return true;
                }
            }
            if (bin_op->left->cast<Declaration>()) {
                const Declaration* decl = bin_op->left->cast<Declaration>();
                if (decl->type == DeclarationType::VAR) {
                    vars_defined.insert(decl->identifier->cast<Identifier>()->name);
                }
                return true;
            }
            return true;
        default:
            throw std::invalid_argument("Unknown operation type");
    }
    return true;
}

bool CodeChecker::visit(const FunctionCall* func) {
    switch(func->type) {
        case FunctionType::PRINT:
        case FunctionType::INPUT:
            return true;
        default:
            throw std::invalid_argument("Unknown function type");
    }
    return true;
}

bool CodeChecker::visit(const Root* tree) {
    return true;
}