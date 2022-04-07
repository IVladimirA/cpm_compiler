#ifndef CPM_COMPILER_ERROR_ERROR_H_
#define CPM_COMPILER_ERROR_ERROR_H_

#include <string>
#include <vector>

class Error {
public:
    Error();
    Error(const std::vector<std::string>& info);
    virtual std::string create_message() const;
protected:
    std::vector<std::string> information;
};

class ConstantRedeclaration : public Error {
public:
    using Error::Error;
    std::string create_message() const override;
};

class VariableRedeclaration : public Error {
public:
    using Error::Error;
    std::string create_message() const override;
};

class UndeclaredIdentifier : public Error {
public:
    using Error::Error;
    std::string create_message() const override;
};
class ConstantRedefinition : public Error {
public:
    using Error::Error;
    std::string create_message() const override;
};
#endif