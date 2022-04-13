#pragma once

#include "node.h"

class Literal : public Node {
public:
    explicit Literal(std::string val);
    void accept(Visitor& visitor) const override;
    const Node* get_last() const override;
    const std::string value;
};

class Identifier : public Node {
public:
    explicit Identifier(std::string val);
    void accept(Visitor& visitor) const override;
    const Node* get_last() const override;
    const std::string name;
};

class Comment : public Node {
public:
    explicit Comment(std::string info);
    void accept(Visitor& visitor) const override;
    const Node* get_last() const override;
    const std::string information;
};
