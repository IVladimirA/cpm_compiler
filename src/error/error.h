#ifndef CPM_COMPILER_ERROR_ERROR_H_
#define CPM_COMPILER_ERROR_ERROR_H_

#include <string>
#include <vector>

class Error {
public:
    virtual std::string get_message() const = 0;
    Error(const std::vector<std::string>& info);
protected:
    const std::vector<std::string> information;
};

class ConstantRedeclaration : public Error {
public:
    ConstantRedeclaration(const std::vector<std::string>& info) : Error(info) {};
    std::string get_message() const override;
};

class VariableRedeclaration : public Error {
public:
    VariableRedeclaration(const std::vector<std::string>& info) : Error(info) {};
    std::string get_message() const override;
};

class UndefinedIdentifier : public Error {
public:
    UndefinedIdentifier(const std::vector<std::string>& info) : Error(info) {};
    std::string get_message() const override;
};

class ConstantRedefinition : public Error {
public:
    ConstantRedefinition(const std::vector<std::string>& info) : Error(info) {};
    std::string get_message() const override;
};

#endif