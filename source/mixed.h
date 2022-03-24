#ifndef MIXED_TYPE_H
#define MIXED_TYPE_H

#include <iostream>
#include <string>

enum MixedType { INT, FLOAT, STR, UNDEF };

class Mixed {
    long long int integer;
    double floating;
    std::string str;
    bool isConst;
    MixedType type;
    public:
        Mixed();
        Mixed(long long int num, bool is_const = false);
        Mixed(int num, bool is_const = false);
        Mixed(double num, bool is_const = false);
        Mixed(std::string string, bool is_const = false);
        friend Mixed operator + (Mixed m1, Mixed const &m2);
        friend Mixed operator - (Mixed m1, Mixed const &m2);
        //friend std::string operator = (Mixed const &m);
        friend std::ostream &operator<<(std::ostream &output, const Mixed &m);
        friend std::istream &operator>>(std::istream& input, Mixed& m);
};

#endif