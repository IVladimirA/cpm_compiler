#include "error.h"

Error::Error(const std::vector<std::string>& info)
    : information{info} {
}

std::string ConstantRedeclaration::get_message() const {
    return "redeclaration of constant \'" + information[0] + "\'";
}

std::string VariableRedeclaration::get_message() const {
    return "redeclaration of variable \'" + information[0] + "\'";
}

std::string UndefinedIdentifier::get_message() const {
    return "usage of undeclared identifier \'" + information[0] + "\'";
}

std::string ConstantRedefinition::get_message() const {
    return "redefinition of constant \'" + information[0] + "\'";
}