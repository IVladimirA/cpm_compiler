#include <iostream>
#include "mixed.h"

Mixed::Mixed() {
    type = UNDEF;
}

Mixed::Mixed(long long int num) {
    type = INT;
    integer = num;
}

Mixed::Mixed(int num) {
    type = INT;
    integer = num;
}

Mixed::Mixed(double num) {
    type = FLOAT;
    floating = num;
}

Mixed::Mixed(const std::string& string) {
    type = STR;
    str = string;
}

Mixed operator+(const Mixed& m1, const Mixed& m2) {
    Mixed result;
    if (m1.type == UNDEF || m2.type == UNDEF) {
        throw std::invalid_argument("Mixed operator+(const Mixed&, const Mixed&) arguments cannot have type \"UNDEF\"");
    }
    if (m1.type == STR || m2.type == STR) {
        result = Mixed((m1.operator std::string()) + (m2.operator std::string()));
    } else {
        switch (m1.type) {
            case INT:
                if (m2.type == INT)
                    result = Mixed(m1.integer + m2.integer);
                else
                    result = Mixed(m1.integer + m2.floating);
                break;
            case FLOAT:
                if (m2.type == INT)
                    result = Mixed(m1.floating + m2.integer);
                else
                    result = Mixed(m1.floating + m2.floating);
                break;
            default:
                break;
        }
    }
    return result;
}

Mixed operator-(const Mixed& m1, const Mixed& m2) {
    if (m1.type == UNDEF || m2.type == UNDEF) {
        throw std::invalid_argument("Mixed operator-(const Mixed&, const Mixed&) arguments cannot have type \"UNDEF\"");
    }
    Mixed left, right;
    switch (m1.type) {
        case STR:
            switch(is_numeric(m1.str)) {
                case 1:
                    left = Mixed(std::stoll(m1.str));
                    break;
                case 2:
                    left = Mixed(std::stod(m1.str));
                    break;
                case 0:
                default:
                    left = Mixed(0);
                    break;
            }
            break;
        case INT:
        case FLOAT:
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
                case 0:
                default:
                    right = Mixed(0);
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
            right = Mixed(0);
            break;
    }
    return left + right;
}

Mixed::operator std::string() const {
    std::string strMixed;
    switch (type) {
        case STR:
            strMixed = str;
            break;
        case INT:
            strMixed = std::to_string(integer);
            break;
        case FLOAT:
            strMixed = std::to_string(floating);
            strMixed.erase(strMixed.find_last_not_of('0') + 1, std::string::npos);
            if (strMixed.back() == '.')
                strMixed.push_back('0');
            break;
        case UNDEF:
        default:
            strMixed = "STR ERROR";
            break;
    }
    return strMixed;
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
    Mixed result;
    std::cout << m.operator std::string();
    std::cin >> value;
    result = Mixed(value);
    return result;
}
