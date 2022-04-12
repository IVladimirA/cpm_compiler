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

CodeGenerator::CodeGenerator() {}

std::string CodeGenerator::get_code() {
    return code;
}

bool CodeGenerator::clear() {
    code = "";
    seen.clear();
    return true;
}
void CodeGenerator::check_last(const Node* tree) {
    if (tree->cast<Literal>()) {
        check_last(tree->cast<Literal>());
        return;
    }
    if (tree->cast<Identifier>()) {
        check_last(tree->cast<Identifier>());
        return;
    }
    if (tree->cast<Comment>()) {
        check_last(tree->cast<Comment>());
        return;
    }
    if (tree->cast<Statement>()) {
        check_last(tree->cast<Statement>());
        return;
    }
    if (tree->cast<Declaration>()) {
        check_last(tree->cast<Declaration>());
        return;
    }
    if (tree->cast<BinaryOperation>()) {
        check_last(tree->cast<BinaryOperation>());
        return;
    }
    if (tree->cast<FunctionCall>()) {
        check_last(tree->cast<FunctionCall>());
        return;
    }
    throw std::invalid_argument("Unknown Node type");
}

bool CodeGenerator::visit(const Node* tree) {
    return Visitor::visit(tree);
}

bool CodeGenerator::visit(const Literal* lit) {
    seen.back().second.push_back("Mixed(" + lit->value + ")");
    check_last(lit);
    return true;
}
void CodeGenerator::check_last(const Literal* lit) {
    if (!seen.empty() && seen.back().first->get_last()->cast<Literal>() == lit) {
        check_last(seen.back().first);
    }
}

bool CodeGenerator::visit(const Identifier* id) {
    seen.back().second.push_back(id->name);
    check_last(id);
    return true;
}
void CodeGenerator::check_last(const Identifier* id) {
    if (!seen.empty() && seen.back().first->get_last()->cast<Identifier>() == id) {
        check_last(seen.back().first);
    }
}

bool CodeGenerator::visit(const Comment* comm) {
    code += comm->information;
    //std::cout << "Comment: " << code << "\n";
    //seen.back().second.push_back(comm->information);
    //check_last(comm);
    return true;
}
void CodeGenerator::check_last(const Comment* comm) {
    if (!seen.empty() && seen.back().first->get_last()->cast<Comment>() == comm) {
        //check_last(seen.back().first);
    }
}

bool CodeGenerator::visit(const Statement* st) {
    //std::cout << "St visitor\n";
    seen.push_back({st, {}});
    return true;
}
void CodeGenerator::check_last(const Statement* st) {
    code += "\n" + seen.back().second[0] + ";";
    std::cout << "Statment: " << code << "\n";
    seen.pop_back();
    /*
    if (!seen.empty() && seen.back().first->get_last()->cast<Statement>() == st) {
        check_last(seen.back().first->get_last());
    }*/
}

bool CodeGenerator::visit(const Declaration* decl) {
    //std::cout << "Decl visitor\n";
    seen.push_back({decl, {}});
    return true;
}

void CodeGenerator::check_last(const Declaration* decl) {
    std::vector<std::string> child_code = seen.back().second;
    seen.pop_back();
    seen.back().second.push_back("Mixed " + child_code[0]);
    if (!seen.empty() && seen.back().first->get_last()->cast<Declaration>() == decl) {
        check_last(seen.back().first);
    }
}

bool CodeGenerator::visit(const BinaryOperation* bin_op) {
    //std::cout << "BinOp visitor\n";
    //std::cout << seen.size() << '\n';
    seen.push_back({bin_op, {}});
    return true;
}

void CodeGenerator::check_last(const BinaryOperation* bin_op) {
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
            break;
    }
    if (!seen.empty() && seen.back().first->get_last()->cast<BinaryOperation>() == bin_op) {
        check_last(seen.back().first);
    }
}

bool CodeGenerator::visit(const FunctionCall* func) {
    seen.push_back({func, {}});
    return true;
}
void CodeGenerator::check_last(const FunctionCall* func) {
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
            break;
    }
    if (!seen.empty() && seen.back().first->get_last()->cast<FunctionCall>() == func) {
        check_last(seen.back().first);
    }
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