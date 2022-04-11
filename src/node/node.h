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
    std::string get_value() const;
    void accept(Visitor& visitor) const override;
private:
    const std::string value;
};

class Identifier : public Node {
public:
    Identifier(const std::string& string);
    std::string get_name() const;
    void accept(Visitor& visitor) const override;
private:
    const std::string name;
};

class Comment : public Node {
public:
    Comment(const std::string& info);
    std::string get_information() const;
    void accept(Visitor& visitor) const override;
private:
    const std::string information;
};

class Statement : public Node {
public:
    Statement(const Node* comm);
    const Node* get_command() const;
    void accept(Visitor& visitor) const override;
    ~Statement();
private:
    const Node* command;
};

enum DeclarationType {
    const_decl,
    var_decl
};

class Declaration : public Node {
public:
    Declaration(DeclarationType t, const Node* id);
    const Node* get_identifier() const;
    DeclarationType get_type() const;
    void accept(Visitor& visitor) const override;
    ~Declaration();
private:
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
    const Node* get_left() const;
    const Node* get_right() const;
    BinOpType get_type() const;
    void accept(Visitor& visitor) const override;
    ~BinaryOperation();
private:
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
    const Node* get_arg() const;
    UnaryArgFuncType get_type() const;
    void accept(Visitor& visitor) const override;
    ~UnaryArgFunction();
private:
    const UnaryArgFuncType type;
    const Node* argument;
};
