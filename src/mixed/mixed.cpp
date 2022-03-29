#include <iostream>
#include "mixed.h"

Mixed::Mixed()
    : type {UNDEF}, integer{0}, floating {0}, str{""} {
}

Mixed::Mixed(long long int num)
    : type {INT}, integer{num}, floating {0}, str{""} {
}

Mixed::Mixed(int num)
    : type {INT}, integer{num}, floating {0}, str{""} {
}

Mixed::Mixed(double num)
    : type {FLOAT}, integer{0}, floating {num}, str{""} {
}

Mixed::Mixed(const std::string& string)
    : type {STR}, integer{0}, floating {0}, str{string} {
}

Mixed operator+(const Mixed& m1, const Mixed& m2) {
    if (m1.type == UNDEF || m2.type == UNDEF) {
        throw std::invalid_argument("Mixed argument has type \"UNDEF\"");
    }
    if (m1.type == STR || m2.type == STR) {
        return Mixed((m1.operator std::string()) + (m2.operator std::string()));
    }
    switch (m1.type) {
        case INT:
            if (m2.type == INT)
                return Mixed(m1.integer + m2.integer);
            return Mixed(m1.integer + m2.floating);
        case FLOAT:
            if (m2.type == INT)
                return Mixed(m1.floating + m2.integer);
            return Mixed(m1.floating + m2.floating);
        default:
            return Mixed();
    }
}

Mixed operator-(const Mixed& m1, const Mixed& m2) {
    if (m1.type == UNDEF || m2.type == UNDEF) {
        throw std::invalid_argument("Mixed argument has type \"UNDEF\"");
    }
    Mixed left(0), right(0);
    switch (m1.type) {
        case STR:
            switch(is_numeric(m1.str)) {
                case 1:
                    left = Mixed(std::stoll(m1.str));
                    break;
                case 2:
                    left = Mixed(std::stod(m1.str));
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
        case STR:
            switch(is_numeric(m2.str)) {
                case 1:
                    right = Mixed(-std::stoll(m2.str));
                    break;
                case 2:
                    right = Mixed(-std::stod(m2.str));
                    break;
                default:
                    break;
            }
            break;
        case INT:
            right = Mixed(-m2.integer);
            break;
        case FLOAT:
            right = Mixed(-m2.floating);
            break;
        default:
            break;
    }
    return left + right;
}

Mixed::operator std::string() const {
    switch (type) {
        default:
            return str;
        case INT:
            return std::to_string(integer);
        case FLOAT:
            std::string str_float = std::to_string(floating);
            str_float.erase(str_float.find_last_not_of('0') + 1, std::string::npos);
            if (str_float.back() == '.')
                str_float.push_back('0');
            return str_float;
    }
}

/* Checks if string numeric */
/* Return: 0 - nonnumeric, 1 - integer, 2 - floating */

int is_numeric(const std::string& s) {
    if (s == "" || s == ".")
        return 0;
    bool point = false;
    for (const char& c : s) {
        if (!std::isdigit(c) && c != '.')
            return 0;
        else if (c == '.') {
            if (point)
                return 0;
            point = true;
        }
    }
    if (point)
        return 2;
    return 1;
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
