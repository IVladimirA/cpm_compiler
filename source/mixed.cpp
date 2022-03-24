#include "mixed.h"

Mixed::Mixed() {
    type = UNDEF;
    isConst = false;
}
Mixed::Mixed(long long int num, bool is_const) {
    type = INT;
    integer = num;
    isConst = is_const;
}
Mixed::Mixed(int num, bool is_const) {
    type = INT;
    integer = num;
    isConst = is_const;
}
Mixed::Mixed(double num, bool is_const) {
    type = FLOAT;
    floating = num;
    isConst = is_const;
}
Mixed::Mixed(std::string string, bool is_const) {
    type = STR;
    str = string;
    isConst = is_const;
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
/*std::string operator = (Mixed const &m) {
    switch (m.type) {
        case INT:
            return std::to_string(m.integer);
        case FLOAT:
            return std::to_string(m.floating);
        case STR:
            return m.str;
        case UNDEF:
            return "Undefined Mixed variable";
        default:
            return "Incorrect string of \"Mixed\" type";
    }
}*/
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