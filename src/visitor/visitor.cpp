#include "visitor.h"
#include "../error/error.h"
#include "../node/node.h"


void Visitor::visit(const Node* tree) {
    switch(tree->get_node_type()) {
        case node_literal:
            visit(static_cast<const Literal*>(tree));
            break;
        case node_identifier:
            visit(static_cast<const Identifier*>(tree));
            break;
        case node_comment:
            visit(static_cast<const Comment*>(tree));
            break;
        case node_statement:
            visit(static_cast<const Statement*>(tree));
            break;
        case node_declaration:
            visit(static_cast<const Declaration*>(tree));
            break;
        case node_bin_op:
            visit(static_cast<const BinaryOperation*>(tree));
            break;
        case node_un_arg_func:
            visit(static_cast<const UnaryArgFunction*>(tree));
            break;
        default:
            throw std::invalid_argument("Unknown node type");
    }
}

CodeGenerator::CodeGenerator(std::string* st) {
    statement = st;
}

void CodeGenerator::update_string(std::string* st) {
    statement = st;
}

void CodeGenerator::visit(const Node* tree) {
    Visitor::visit(tree);
}

void CodeGenerator::visit(const Literal* lit) {
    *statement += "Mixed(" + lit->get_value() + ")";
    
}

void CodeGenerator::visit(const Identifier* id) {
    *statement += id->get_name();
}

void CodeGenerator::visit(const Comment* comm) {
    *statement += comm->get_information();
}

void CodeGenerator::visit(const Statement* st) {
    *statement += "\n";
    st->get_command()->accept(*this);
    *statement += ";";
}

void CodeGenerator::visit(const Declaration* decl) {
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
}

void CodeGenerator::visit(const BinaryOperation* bin_op) {
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
}

void CodeGenerator::visit(const UnaryArgFunction* func) {
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
    vars_defined = vars_defined;
}

void CodeChecker::visit(const Node* tree) {
    Visitor::visit(tree);
}

void CodeChecker::visit(const Literal* lit) {
        
}

void CodeChecker::visit(const Identifier* id) {
    if (consts->find(id->get_name()) == consts->end()
        && vars_defined->find(id->get_name()) == vars_defined->end()) {
        errors->push_back(new UndefinedIdentifier({id->get_name()}));
    }
}

void CodeChecker::visit(const Comment* comm) {
    
}

void CodeChecker::visit(const Statement* st) {
    visit(st->get_command());
}

void CodeChecker::visit(const Declaration* decl) {
    const Identifier* id = static_cast<const Identifier*>(decl->get_identifier());
    if (consts->find(id->get_name()) != consts->end()) {
        errors->push_back(new ConstantRedeclaration({id->get_name()}));
        return;
    }
    if (vars_declared->find(id->get_name()) != vars_declared->end()) {
        errors->push_back(new VariableRedeclaration({id->get_name()}));
        return;
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
}

void CodeChecker::visit(const BinaryOperation* bin_op) {
    visit(bin_op->get_right());
    switch(bin_op->get_type()) {
        case addition_op:
        case subtraction_op:
            visit(bin_op->get_left());
            break;
        case assignment_op:
            if (bin_op->get_left()->get_node_type() == node_identifier) {
                const Identifier* id = static_cast<const Identifier*>(bin_op->get_left());
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
            if (errors->size() == 0 && bin_op->get_left()->get_node_type() == node_declaration) {
                const Declaration* decl = static_cast<const Declaration*>(bin_op->get_left());
                if (decl->get_type() == var_decl) {
                    vars_defined->insert(static_cast<const Identifier*>(decl->get_identifier())->get_name());
                }
            }
            break;
        default:
            throw std::invalid_argument("Unknown operation type");
    }
}

void CodeChecker::visit(const UnaryArgFunction* func) {
    switch(func->get_type()) {
        case un_f_input:
        case un_f_print:
            visit(func->get_arg());            
            break;
        default:
            throw std::invalid_argument("Unknown function type");
    }
}