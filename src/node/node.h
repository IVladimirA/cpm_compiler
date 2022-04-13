#pragma once

#include <unordered_set>
#include <string>
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
