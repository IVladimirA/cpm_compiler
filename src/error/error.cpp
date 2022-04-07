#include "error.h"

Error::Error() {
}

Error::Error(const std::vector<std::string>& info)
    : information{info} {
}

std::string Error::create_message() const {
    std::string message = "";
    for (const std::string& block : information) {
        message += block;
    }
    return message;
}

std::string ConstantRedeclaration::create_message() const {
    return "redeclaration of constant \'" + information[0] + "\'";
}

std::string VariableRedeclaration::create_message() const {
    return "redeclaration of variable \'" + information[0] + "\'";
}

std::string UndeclaredIdentifier::create_message() const {
    return "usage of undeclared identifier \'" + information[0] + "\'";
}

std::string ConstantRedefinition::create_message() const {
    return "redefinition of constant \'" + information[0] + "\'";
}