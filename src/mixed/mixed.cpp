#include <iostream>
#include "mixed.h"

Mixed::Mixed()
    : type{dt_undef}, integer{0}, floating{0}, string{""} {
}

Mixed::Mixed(long long int num)
    : type{dt_int}, integer{num}, floating{0}, string{""} {
}

Mixed::Mixed(int num)
    : type{dt_int}, integer{num}, floating{0}, string{""} {
}

Mixed::Mixed(double num)
    : type{dt_float}, integer{0}, floating{num}, string{""} {
}

Mixed::Mixed(const std::string& string)
    : type{dt_string}, integer{0}, floating{0}, string{string} {
}

bool Mixed::is_defined() const {
    return type != dt_undef;
}

Mixed::operator std::string() const {
    if (!is_defined()) {
        throw std::logic_error("Undefined data type");
    }
    switch (type) {
        case dt_int:
            return std::to_string(integer);
        case dt_float: {
            std::string str_float = std::to_string(floating);
            str_float.erase(str_float.find_last_not_of('0') + 1, std::string::npos);
            if (str_float.back() == '.')
                str_float.push_back('0');
            return str_float;
        }
        case dt_string:
            return string;
        default:
            throw std::logic_error("Unknown data type");
    }
}

StringType is_numeric(const std::string& s) {
    if (s == "" || s == ".")
        return str_nonnumeric;
    bool point = false;
    for (const char& c : s) {
        if (!std::isdigit(c) && c != '.')
            return str_nonnumeric;
        else if (c == '.') {
            if (point)
                return str_nonnumeric;
            point = true;
        }
    }
    if (point)
        return str_float;
    return str_int;
}

Mixed to_numeric(const Mixed& m) {
    if (!m.is_defined()) {
        throw std::logic_error("Undefined data type");
    }
    switch (m.type) {
        case dt_string:
            switch(is_numeric(m.string)) {
                case str_int:
                    return Mixed(std::stoll(m.string));
                case str_float:
                    return Mixed(std::stod(m.string));
                case str_nonnumeric:
                    return Mixed(0);
                default:
                    throw std::logic_error("Unknown str type");
            }
        case dt_int:
            return Mixed(m.integer);
        case dt_float:
            return Mixed(m.floating);
        default:
            throw std::logic_error("Unknown data type");;
    }
}

Mixed operator+(const Mixed& m1, const Mixed& m2) {
    if (!m1.is_defined() || !m2.is_defined()) {
        throw std::invalid_argument("Mixed argument undefined");
    }
    if (m1.type == dt_string || m2.type == dt_string) {
        return Mixed((m1.operator std::string()) + (m2.operator std::string()));
    }
    if (m1.type == dt_int && m2.type == dt_int) {
        return Mixed(m1.integer + m2.integer);
    }
    if (m1.type == dt_int && m2.type == dt_float) {
        return Mixed(m1.integer + m2.floating);
    }
    if (m1.type == dt_float && m2.type == dt_int) {
        return Mixed(m1.floating + m2.integer);
    }
    if (m1.type == dt_float && m2.type == dt_float) {
        return Mixed(m1.floating + m2.floating);
    }
    throw std::invalid_argument("Invalid pair of Mixed datatypes");
}

Mixed operator-(const Mixed& m1, const Mixed& m2) {
    if (!m1.is_defined() || !m2.is_defined()) {
        throw std::invalid_argument("Mixed argument undefined");
    }
    Mixed left = to_numeric(m1);
    Mixed right = to_numeric(m2);
    if (right.type == dt_int) {
        right = Mixed(-right.integer);
    }
    if (right.type == dt_float) {
        right = Mixed(-right.floating);
    }
    return left + right;
}

void print(const Mixed& m) {
    std::cout << m.operator std::string() << '\n';
}

Mixed input(const Mixed& m) {
    std::string value;
    std::cout << m.operator std::string();
    std::getline(std::cin, value);
    return Mixed(value);
}
