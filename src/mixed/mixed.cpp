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

Mixed operator+(const Mixed& m1, const Mixed& m2) {
    if (m1.type == dt_undef || m2.type == dt_undef) {
        throw std::invalid_argument("Mixed argument has type \"UNDEF\"");
    }
    if (m1.type == dt_string || m2.type == dt_string) {
        return Mixed((m1.operator std::string()) + (m2.operator std::string()));
    }
    switch (m1.type) {
        case dt_int:
            if (m2.type == dt_int)
                return Mixed(m1.integer + m2.integer);
            return Mixed(m1.integer + m2.floating);
        case dt_float:
            if (m2.type == dt_int)
                return Mixed(m1.floating + m2.integer);
            return Mixed(m1.floating + m2.floating);
        default:
            return Mixed();
    }
}

Mixed operator-(const Mixed& m1, const Mixed& m2) {
    if (m1.type == dt_undef || m2.type == dt_undef) {
        throw std::invalid_argument("Mixed argument has type \"UNDEF\"");
    }
    Mixed left(0), right(0);
    switch (m1.type) {
        case dt_string:
            switch(is_numeric(m1.string)) {
                case str_int:
                    left = Mixed(std::stoll(m1.string));
                    break;
                case str_float:
                    left = Mixed(std::stod(m1.string));
                    break;
                default:
                    break;
            }
            break;
        default:
            left = m1;
            break;
    }
    switch (m2.type) {
        case dt_string:
            switch(is_numeric(m2.string)) {
                case str_int:
                    right = Mixed(-std::stoll(m2.string));
                    break;
                case str_float:
                    right = Mixed(-std::stod(m2.string));
                    break;
                default:
                    break;
            }
            break;
        case dt_int:
            right = Mixed(-m2.integer);
            break;
        case dt_float:
            right = Mixed(-m2.floating);
            break;
        default:
            break;
    }
    return left + right;
}

Mixed::operator std::string() const {
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
        default:
            return string;
    }
}

// Checks if string numeric
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

void print(const Mixed& m) {
    std::cout << m.operator std::string() << '\n';
}

Mixed input(const Mixed& m) {
    std::string value;
    std::cout << m.operator std::string();
    std::getline(std::cin, value);
    return Mixed(value);
}
