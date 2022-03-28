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
Mixed::Mixed(std::string string) {
    type = STR;
    str = string;
}

Mixed::Mixed(const char* string) {
    type = STR;
    str = string;
}

Mixed operator + (Mixed m1, Mixed const &m2) {
    Mixed result;
    if (m1.type == UNDEF || m2.type == UNDEF) {
        throw std::invalid_argument("Operator + cannot take UNDEF type Mixed as an argument");
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
Mixed operator - (Mixed m1, Mixed const &m2) {
    Mixed left, right;
    switch (m1.type) {
        case STR:
            try {
                left = Mixed(std::stod(m1.str));
            } catch (const std::exception& e) {
                try {
                    left = Mixed(std::stoll(m1.str));
                } catch (const std::exception& e) {
                    left = Mixed(0);
                }
            }
            break;
        case INT:
        case FLOAT:
            left = m1;
            break;
        default:
            left = Mixed(0);        
    }
    switch (m2.type) {
        case STR:
            try {
                right = Mixed(std::stod(m2.str));
            } catch (const std::exception& e) {
                try {
                    right = Mixed(std::stoll(m2.str));
                } catch (const std::exception& e) {
                    right = Mixed(0);
                }
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
