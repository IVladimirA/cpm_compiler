#ifndef MIXED_TYPE_H
#define MIXED_TYPE_H

#include <string>

enum MixedType { INT, FLOAT, STR, UNDEF };

class Mixed {
    long long int integer;
    double floating;
    std::string str;
    MixedType type;
    public:
        Mixed();
        Mixed(long long int num);
        Mixed(int num);
        Mixed(double num);
        Mixed(std::string string);
        friend Mixed operator+(Mixed m1, Mixed const &m2);
        friend Mixed operator-(Mixed m1, Mixed const &m2);
        Mixed& operator=(const std::string& s);
        //friend std::string operator = (Mixed const &m);
        operator std::string();
        //friend operator Mixed(std::string s);
        friend std::ostream &operator<<(std::ostream &output, const Mixed &m);
        friend std::istream &operator>>(std::istream& input, Mixed& m);
};

#endif