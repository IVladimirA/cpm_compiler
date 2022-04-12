#pragma once

#include <unordered_set>
#include <vector>

class Visitor;

class Node {
public:
    virtual void accept(Visitor& visitor) const = 0;
    virtual ~Node() = default;
    virtual const Node* get_last() const = 0;
    template<typename T>
    const T* cast() const {
        return dynamic_cast<const T*>(this);
    }
};

class Literal : public Node {
public:
    Literal(const std::string& string);
    void accept(Visitor& visitor) const override;
    const Node* get_last() const override;
    const std::string value;
};

class Identifier : public Node {
public:
    Identifier(const std::string& string);
    void accept(Visitor& visitor) const override;
    const Node* get_last() const override;
    const std::string name;
};

class Comment : public Node {
public:
    Comment(const std::string& info);
    void accept(Visitor& visitor) const override;
    const Node* get_last() const override;
    const std::string information;
};

class Statement : public Node {
public:
    Statement(const Node* comm);
    void accept(Visitor& visitor) const override;
    ~Statement();
    const Node* get_last() const override;
    const Node* command;
};

enum class DeclarationType {
    CONST,
    VAR
};

class Declaration : public Node {
public:
    Declaration(DeclarationType t, const Node* id);
    void accept(Visitor& visitor) const override;
    ~Declaration();
    const Node* get_last() const override;
    const DeclarationType type;
    const Node* identifier;
};

enum class BinOpType {
    ADDITION,
    SUBTRACTION,
    ASSIGNMENT
};

class BinaryOperation : public Node {
public:
    BinaryOperation(BinOpType op, const Node* l, const Node* r);
    void accept(Visitor& visitor) const override;
    ~BinaryOperation();
    const Node* get_last() const override;
    const BinOpType type;
    const Node* left;
    const Node* right;
};

enum class FunctionType {
    PRINT,
    INPUT
};

class FunctionCall : public Node {
public:
    FunctionCall(FunctionType t, std::vector<const Node*> args);
    void accept(Visitor& visitor) const override;
    ~FunctionCall();
    const Node* get_last() const override;
    const FunctionType type;
    std::vector<const Node*> arguments;
};
