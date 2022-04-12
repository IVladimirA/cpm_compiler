#include <iostream>
#include "mixed.h"

Mixed::Mixed() // Мелочь, но кажется тут и далее без переноса было бы по приятнее.
    : type(DataType::UNDEF), integer(0), floating(0), string("") {
}

Mixed::Mixed(long long int num) // А строку тут, кстати, необязательно инициализировать (Clang-Tidy: Redundant string initialization)
    : type{DataType::INT}, integer(num), floating(0), string("") {
}

Mixed::Mixed(int num) // А чтобы не инициализировать каждый раз int и float, можно прямо в определении класса написать = 0 и = 0.0
    : type(DataType::INT), integer(num), floating(0), string("") {
}

Mixed::Mixed(double num)
    : type(DataType::FLOAT), integer(0), floating(num), string("") {
}

// Clang-Tidy: Pass by value and use std::move
Mixed::Mixed(const std::string& string)
    : type(DataType::STRING), integer(0), floating(0), string(string) {
}

bool Mixed::is_defined() const {
    return type != DataType::UNDEF;
}

std::string Mixed::to_string() const {
    if (!is_defined()) {
        throw std::logic_error("Undefined Mixed");
    }
    switch (type) {
        case DataType::INT:
            return std::to_string(integer);
        case DataType::FLOAT: {
            std::string str_float = std::to_string(floating);
            str_float.erase(str_float.find_last_not_of('0') + 1, std::string::npos);
            if (str_float.back() == '.')
                str_float.push_back('0');
            return str_float;
        }
        case DataType::STRING:
            return string;
        default:
            throw std::logic_error("Unknown DataType");
    }
}

StringType is_numeric(const std::string& s) {
    if (s == "" || s == ".")
        return StringType::NONNUMERIC;
    bool was_point = false;
    for (const char& c : s) {
        // Обычно однострочные if у тебя обернуты в фигурные скобеи.
        // А тут нет, не консистентно.
        if (!std::isdigit(c) && c != '.')
            return StringType::NONNUMERIC;
        else if (c == '.') {
            if (was_point)
                return StringType::NONNUMERIC;
            was_point = true;
        }
    }
    if (was_point)
        return StringType::FLOAT;
    return StringType::INT;
}

Mixed Mixed::to_numeric() const {
    if (!is_defined()) {
        throw std::logic_error("Undefined Mixed");
    }
    switch (type) {
        case DataType::STRING:
            switch(is_numeric(string)) {
                case StringType::INT:
                    return {std::stoll(string)};
                case StringType::FLOAT:
                    return {std::stod(string)};
                case StringType::NONNUMERIC:
                    return {0};
                default:
                    throw std::logic_error("Unknown StringType");
            }
        case DataType::INT:
            return {integer};
        case DataType::FLOAT:
            return {floating};
        default:
            throw std::logic_error("Unknown DataType");
    }
}

Mixed operator+(const Mixed& m1, const Mixed& m2) {
    if (!m1.is_defined() || !m2.is_defined()) {
        throw std::invalid_argument("Undefined Mixed");
    }
    if (m1.type == DataType::STRING || m2.type == DataType::STRING) {
        return {m1.to_string() + m2.to_string()};
    }
    if (m1.type == DataType::INT && m2.type == DataType::INT) {
        return {m1.integer + m2.integer};
    }
    if (m1.type == DataType::INT && m2.type == DataType::FLOAT) {
        return {m1.integer + m2.floating};
    }
    if (m1.type == DataType::FLOAT && m2.type == DataType::INT) {
        return {m1.floating + m2.integer};
    }
    if (m1.type == DataType::FLOAT && m2.type == DataType::FLOAT) {
        return {m1.floating + m2.floating};
    }
    throw std::invalid_argument("Invalid pair of Mixed DataTypes");
}

Mixed operator-(const Mixed& m1, const Mixed& m2) {
    if (!m1.is_defined() || !m2.is_defined()) {
        throw std::invalid_argument("Undefined Mixed");
    }
    Mixed left = m1.to_numeric();
    Mixed right = m2.to_numeric();
    if (right.type == DataType::INT) {
        right = {-right.integer};
    }
    if (right.type == DataType::FLOAT) {
        right = {-right.floating};
    }
    return left + right;
}

void print(const Mixed& m) {
    std::cout << m.to_string() << '\n';
}

Mixed input(const Mixed& m) {
    std::string value;
    std::cout << m.to_string();
    std::getline(std::cin, value);
    return {value};
}
