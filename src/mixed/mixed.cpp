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
Mixed operator + (Mixed m1, Mixed const &m2) {
    Mixed result;
    if (m1.type == UNDEF || m2.type == UNDEF) {
        std::cout << "Error: operator \"+\" with indefined variable(s)" << std::endl;
        return result;
    }
    if (m1.type == STR) {
        switch (m2.type) {
            case STR:
                result = Mixed(m1.str + m2.str);
                break;
            case INT:
                result = Mixed(m1.str + std::to_string(m2.integer));
                break;
            case FLOAT:
                result = Mixed(m1.str + std::to_string(m2.floating));
                break;
            default:
                break;
        }
    } else if (m2.type == STR) {
        switch (m1.type) {
            case INT:
                result = Mixed(std::to_string(m1.integer) + m2.str);
                break;
            case FLOAT:
                result = Mixed(std::to_string(m1.integer) + m2.str);
                break;
            default:
                break;
        }
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

Mixed& Mixed::operator=(const std::string& s) {
    type = STR;
    str = s;
    return *this;
}

Mixed::operator std::string() {
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
            break;
        case UNDEF:
        default:
            strMixed = "STR ERROR";
            break;
    }
    return strMixed;
}

std::ostream &operator<<(std::ostream &output, const Mixed &m) {
    switch (m.type) {
        case INT:
            return output << m.integer;
        case FLOAT:
            return output << m.floating;
        case STR:
            return output << m.str;
        case UNDEF:
            return output << "Undefined Mixed variable";
        default:
            return output << "Incorrect output of \"Mixed\" type";
    }
}

std::istream& operator>>(std::istream& input, Mixed& m) {
    std::string s;
    std::cin >> s;
    m = Mixed(s);
    return input;
}
