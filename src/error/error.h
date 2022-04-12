#pragma once
// Пропущена пустая строка.
#include <string>
#include <vector>

class Error {
public:
    virtual std::string get_message() const = 0;
    Error(const std::vector<std::string>& info);
    virtual ~Error() = default;
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
