#include <iostream>
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
    throw std::invalid_argument("Unknown Node type");
}

CodeGenerator::CodeGenerator(std::string* st) {
    statement = st;
}

void CodeGenerator::update_string(std::string* st) {
    statement = st;
}

bool CodeGenerator::visit(const Node* tree) {
    return Visitor::visit(tree);
}

bool CodeGenerator::visit(const Literal* lit) {
    *statement += "Mixed(" + lit->value + ")";
    return true;
}

bool CodeGenerator::visit(const Identifier* id) {
    *statement += id->name;
    return true;
}

bool CodeGenerator::visit(const Comment* comm) {
    *statement += comm->information;
    return true;
}

bool CodeGenerator::visit(const Statement* st) {
    *statement += "\n";
    st->command->accept(*this);
    *statement += ";";
    return true;
}

bool CodeGenerator::visit(const Declaration* decl) {
    switch(decl->type) {
        case DeclarationType::CONST:
            *statement += "const Mixed ";
            break;
        case DeclarationType::VAR:
            *statement += "Mixed ";
            break;
        default:
            throw std::invalid_argument("Unknown declaration type");
    }
    visit(decl->identifier);
    return true;
}

bool CodeGenerator::visit(const BinaryOperation* bin_op) {
    visit(bin_op->left);
    switch(bin_op->type) {
        case BinOpType::ADDITION:
            *statement += " + ";
            break;
        case BinOpType::SUBTRACTION:
            *statement += " - ";
            break;
        case BinOpType::ASSIGNMENT:
           *statement += " = ";
            break;
        default:
            throw std::invalid_argument("Unknown binary operation type");
    }
    visit(bin_op->right);
    return true;
}

bool CodeGenerator::visit(const FunctionCall* func) {
    switch(func->type) {
        case FunctionType::INPUT:
            *statement += "input(";
            break;
        case FunctionType::PRINT:
            *statement += "print(";
            break;
        default:
            throw std::invalid_argument("Unknown function type");
    }
    visit(func->arguments[0]);
    *statement += ")";
    return true;
}


CodeChecker::CodeChecker(
    std::vector<const Error*>* err,
    std::unordered_set<std::string>* con,
    std::unordered_set<std::string>* vars_decl,
    std::unordered_set<std::string>* vars_def)
    : errors{err}, consts{con}, vars_declared{vars_decl}, vars_defined{vars_def} {

}

void CodeChecker::update(
    std::vector<const Error*>* err,
    std::unordered_set<std::string>* con,
    std::unordered_set<std::string>* vars_decl,
    std::unordered_set<std::string>* vars_def) {
    errors = err;
    consts = con;
    vars_declared = vars_decl;
    vars_defined = vars_def;
}

bool CodeChecker::visit(const Node* tree) {
    return Visitor::visit(tree);
}

bool CodeChecker::visit(const Literal* lit) {
    return true;
}

bool CodeChecker::visit(const Identifier* id) {
    if (consts->find(id->name) == consts->end()
        && vars_defined->find(id->name) == vars_defined->end()) {
        errors->push_back(new UndefinedIdentifier({id->name}));
    }
    return true;
}

bool CodeChecker::visit(const Comment* comm) {
    return true;
}

bool CodeChecker::visit(const Statement* st) {
    visit(st->command);
    return true;
}

bool CodeChecker::visit(const Declaration* decl) {
    const Identifier* id = decl->identifier->cast<Identifier>();
    if (consts->find(id->name) != consts->end()) {
        errors->push_back(new ConstantRedeclaration({id->name}));
        return true;
    }
    if (vars_declared->find(id->name) != vars_declared->end()) {
        errors->push_back(new VariableRedeclaration({id->name}));
        return true;
    }
    switch(decl->type) {
        case DeclarationType::CONST:
            consts->insert(id->name);
            break;
        case DeclarationType::VAR:
            vars_declared->insert(id->name);
            break;
        default:
            throw std::invalid_argument("Unknown declaration type");
    }
    return true;
}

bool CodeChecker::visit(const BinaryOperation* bin_op) {
    visit(bin_op->right);
    switch(bin_op->type) {
        case BinOpType::ADDITION:
        case BinOpType::SUBTRACTION:
            visit(bin_op->left);
            break;
        case BinOpType::ASSIGNMENT:
            if (bin_op->left->cast<Identifier>()) {
                const Identifier* id = bin_op->left->cast<Identifier>();
                if (consts->find(id->name) != consts->end()) {
                    errors->push_back(new ConstantRedefinition({id->name}));
                    break;
                }
                if (vars_declared->find(id->name) != vars_declared->end()) {
                    vars_defined->insert(id->name);
                    break;
            }
            }
            visit(bin_op->left);
            if (errors->size() == 0 && bin_op->left->cast<Declaration>()) {
                const Declaration* decl = bin_op->left->cast<Declaration>();
                if (decl->type == DeclarationType::VAR) {
                    vars_defined->insert(decl->identifier->cast<Identifier>()->name);
                }
            }
            break;
        default:
            throw std::invalid_argument("Unknown operation type");
    }
    return true;
}

bool CodeChecker::visit(const FunctionCall* func) {
    switch(func->type) {
        case FunctionType::PRINT:
        case FunctionType::INPUT:
            visit(func->arguments[0]);            
            break;
        default:
            throw std::invalid_argument("Unknown function type");
    }
    return true;
}