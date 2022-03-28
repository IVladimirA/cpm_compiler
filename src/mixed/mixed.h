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
        Mixed(const char* string);
        friend Mixed operator+(Mixed m1, Mixed const &m2);
        friend Mixed operator-(Mixed m1, Mixed const &m2);
        operator std::string() const;
};

void print(const std::string& string = "");
Mixed input(const std::string& string = "");

#endif