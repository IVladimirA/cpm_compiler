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
    if (tree->cast<UnaryArgFunction>()) {
        return visit(tree->cast<UnaryArgFunction>());
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
    *statement += "Mixed(" + lit->get_value() + ")";
    return true;
}

bool CodeGenerator::visit(const Identifier* id) {
    *statement += id->get_name();
    return true;
}

bool CodeGenerator::visit(const Comment* comm) {
    *statement += comm->get_information();
    return true;
}

bool CodeGenerator::visit(const Statement* st) {
    *statement += "\n";
    st->get_command()->accept(*this);
    *statement += ";";
    return true;
}

bool CodeGenerator::visit(const Declaration* decl) {
    switch(decl->get_type()) {
        case const_decl:
            *statement += "const Mixed ";
            break;
        case var_decl:
            *statement += "Mixed ";
            break;
        default:
            throw std::invalid_argument("Unknown declaration type");
    }
    visit(decl->get_identifier());
    return true;
}

bool CodeGenerator::visit(const BinaryOperation* bin_op) {
    visit(bin_op->get_left());
    switch(bin_op->get_type()) {
        case addition_op:
            *statement += " + ";
            break;
        case subtraction_op:
            *statement += " - ";
            break;
        case assignment_op:
           *statement += " = ";
            break;
        default:
            throw std::invalid_argument("Unknown binary operation type");
    }
    visit(bin_op->get_right());
    return true;
}

bool CodeGenerator::visit(const UnaryArgFunction* func) {
    switch(func->get_type()) {
        case un_f_input:
            *statement += "input(";
            break;
        case un_f_print:
            *statement += "print(";
            break;
        default:
            throw std::invalid_argument("Unknown function type");
    }
    visit(func->get_arg());
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
    if (consts->find(id->get_name()) == consts->end()
        && vars_defined->find(id->get_name()) == vars_defined->end()) {
        errors->push_back(new UndefinedIdentifier({id->get_name()}));
    }
    return true;
}

bool CodeChecker::visit(const Comment* comm) {
    return true;
}

bool CodeChecker::visit(const Statement* st) {
    visit(st->get_command());
    return true;
}

bool CodeChecker::visit(const Declaration* decl) {
    const Identifier* id = decl->get_identifier()->cast<Identifier>();
    if (consts->find(id->get_name()) != consts->end()) {
        errors->push_back(new ConstantRedeclaration({id->get_name()}));
        return true;
    }
    if (vars_declared->find(id->get_name()) != vars_declared->end()) {
        errors->push_back(new VariableRedeclaration({id->get_name()}));
        return true;
    }
    switch(decl->get_type()) {
        case const_decl:
            consts->insert(id->get_name());
            break;
        case var_decl:
            vars_declared->insert(id->get_name());
            break;
        default:
            throw std::invalid_argument("Unknown declaration type");
    }
    return true;
}

bool CodeChecker::visit(const BinaryOperation* bin_op) {
    visit(bin_op->get_right());
    switch(bin_op->get_type()) {
        case addition_op:
        case subtraction_op:
            visit(bin_op->get_left());
            break;
        case assignment_op:
            if (bin_op->get_left()->cast<Identifier>()) {
                const Identifier* id = bin_op->get_left()->cast<Identifier>();
                if (consts->find(id->get_name()) != consts->end()) {
                    errors->push_back(new ConstantRedefinition({id->get_name()}));
                    break;
                }
                if (vars_declared->find(id->get_name()) != vars_declared->end()) {
                    vars_defined->insert(id->get_name());
                    break;
            }
            }
            visit(bin_op->get_left());
            if (errors->size() == 0 && bin_op->get_left()->cast<Declaration>()) {
                const Declaration* decl = bin_op->get_left()->cast<Declaration>();
                if (decl->get_type() == var_decl) {
                    vars_defined->insert(decl->get_identifier()->cast<Identifier>()->get_name());
                }
            }
            break;
        default:
            throw std::invalid_argument("Unknown operation type");
    }
    return true;
}

bool CodeChecker::visit(const UnaryArgFunction* func) {
    switch(func->get_type()) {
        case un_f_input:
        case un_f_print:
            visit(func->get_arg());            
            break;
        default:
            throw std::invalid_argument("Unknown function type");
    }
    return true;
}