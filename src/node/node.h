#pragma once

#include <unordered_set>

class Visitor;

class Node {
public:
    virtual void accept(Visitor& visitor) const = 0;
    virtual ~Node() = default;
    template<typename T>
    const T* cast() const {
        return dynamic_cast<const T*>(this);
    }
};

class Literal : public Node {
public:
    Literal(const std::string& string);
    void accept(Visitor& visitor) const override;
    const std::string value;
};

class Identifier : public Node {
public:
    Identifier(const std::string& string);
    void accept(Visitor& visitor) const override;
    const std::string name;
};

class Comment : public Node {
public:
    Comment(const std::string& info);
    void accept(Visitor& visitor) const override;
    const std::string information;
};

class Statement : public Node {
public:
    Statement(const Node* comm);
    void accept(Visitor& visitor) const override;
    ~Statement();
    const Node* command;
};

enum DeclarationType {
    const_decl,
    var_decl
};

class Declaration : public Node {
public:
    Declaration(DeclarationType t, const Node* id);
    void accept(Visitor& visitor) const override;
    ~Declaration();
    const DeclarationType type;
    const Node* identifier;
};

enum BinOpType {
    addition_op,
    subtraction_op,
    assignment_op
};

class BinaryOperation : public Node {
public:
    BinaryOperation(BinOpType op, const Node* l, const Node* r);
    void accept(Visitor& visitor) const override;
    ~BinaryOperation();
    const BinOpType type;
    const Node* left;
    const Node* right;
};

enum UnaryArgFuncType {
    un_f_print,
    un_f_input
};

class UnaryArgFunction : public Node {
public:
    UnaryArgFunction(UnaryArgFuncType t, const Node* arg);
    void accept(Visitor& visitor) const override;
    ~UnaryArgFunction();
    const UnaryArgFuncType type;
    const Node* argument;
};
