// Но ведь есть #pragma once, который более читаем.
#ifndef CPM_COMPILER_NODE_NODE_H_
#define CPM_COMPILER_NODE_NODE_H_

// Кажется ты потерял библиотеку, без нее не компилировалось.
#include <string>
#include <unordered_set>

// Лучше использовать enum class, таким образом ты не загрязняешь
// глобальный скоуп.
// Также тогда не нужен был бы префикс node_.
// Также насколько я знаю обычно по кодстайлу перечисления пишутся
// в SCREAMING_SNAKE_CASE.
//
// В итоге я бы написал это примерно так:
// enum class NodeType {
//   LITERAL,
//   BIN_OP,
//   // ...
// };
enum NodeType {
    node_literal,
    node_identifier,
    node_comment,
    node_statement,
    node_declaration,
    node_bin_op,
    node_un_arg_func
};

class Visitor;

class Node {
public:
    virtual void accept(Visitor& visitor) const = 0;
    // А зачем тебе вообще типы узлов если у тебя уже есть узлы
    // наследники от Node?
    // Можно типы проверять вот так:
    // auto node = bin_op->get_left();
    // if (const auto* literal = dynamic_cast<const Literal*>(node)) {
    //    // do something
    // } else if (const auto* identifier = dynamic_cast<const Identifier*>(node)) {
    //    // do something
    // }
    //
    // Но я бы еще тут написал функцию cast для удобной проверки. см. ниже.
    // Тогда код был бы короче:
    // if (const auto* literal = node.cast<Literal>()) {
    //    // do something
    // } else if (const auto* identifier = node.cast<Identifier>()) {
    //    // do something
    // }
    virtual NodeType get_node_type() const = 0;

    // Для примера, не тестировал, что это 100% корректная реализация.
    template<typename T>
    const T* cast() {
       return dynamic_cast<const T*>(this);
    }
};

class Literal : public Node {
public:
    Literal(const std::string& string); // Clang-Tidy: Single-argument constructors must be marked explicit to avoid unintentional implicit conversions

    // Я вообще не уверен нужны ли эти геттеры и сокрытие полей.
    // Мне кажется весь код выглядел бы приятнее если бы все
    // данные узла были public.
    std::string get_value() const;
    void accept(Visitor& visitor) const override;
    NodeType get_node_type() const override;
private:
    const std::string value;
};

// Мне кажется тут лучше подходит имя Variable.
class Identifier : public Node {
public:
    Identifier(const std::string& string);
    std::string get_name() const;
    void accept(Visitor& visitor) const override;
    NodeType get_node_type() const override;
private:
    const std::string name;
};

class Comment : public Node {
public:
    Comment(const std::string& info);
    std::string get_information() const;
    void accept(Visitor& visitor) const override;
    NodeType get_node_type() const override;
private:
    const std::string information;
};

class Statement : public Node {
public:
    Statement(const Node* comm);
    const Node* get_command() const;
    void accept(Visitor& visitor) const override;
    NodeType get_node_type() const override;
    ~Statement();
private:
    const Node* command;
};

// Тоже, что и для NodeType.
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
    NodeType get_node_type() const override;
    ~Declaration();
private:
    const DeclarationType type;
    const Node* identifier;
};

// Тоже, что и для NodeType.
enum BinOpType {
    addition_op,
    subtraction_op,
    assignment_op
};

class BinaryOperation : public Node {
public:
    // Одно буквенные названия переменных лучше избегать.
    BinaryOperation(BinOpType op, const Node* l, const Node* r);
    const Node* get_left() const;
    const Node* get_right() const;
    BinOpType get_type() const;
    void accept(Visitor& visitor) const override;
    NodeType get_node_type() const override;
    ~BinaryOperation();
private:
    const BinOpType type;
    const Node* left;
    const Node* right;
};

// Тоже, что и для NodeType.
enum UnaryArgFuncType {
    // Не лучшие названия, конечно, с enum class думаю их можно будет
    // сократить до PRINT и INPUT.
    un_f_print,
    un_f_input
};

// Как по мне не лучшее название, лучше FunctionCall.
class UnaryArgFunction : public Node {
public:
    UnaryArgFunction(UnaryArgFuncType t, const Node* arg);
    const Node* get_arg() const;
    UnaryArgFuncType get_type() const;
    void accept(Visitor& visitor) const override;
    NodeType get_node_type() const override;
    ~UnaryArgFunction();
private:
    const UnaryArgFuncType type;
    // В случае если назвать FunctionCall, то тут хотелось
    // бы видеть вектор узлов, но это к слову просто.
    const Node* argument;
};

#endif