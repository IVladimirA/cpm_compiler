#ifndef CPM_COMPILER_MIXED_MIXED_H_
#define CPM_COMPILER_MIXED_MIXED_H_

#include <string>

enum MixedType { INT, FLOAT, STR, UNDEF };

class Mixed {
public:
    Mixed();
    Mixed(long long int num);
    Mixed(int num);
    Mixed(double num);
    Mixed(std::string string);
    friend Mixed operator+(const Mixed& m1, const Mixed& m2);
    friend Mixed operator-(const Mixed& m1, const Mixed& m2);
    operator std::string() const;
private:
    long long int integer;
    double floating;
    std::string str;
    MixedType type;
};

int is_numeric(const std::string& s);
void print(const Mixed& m);
Mixed input(const Mixed& m);

#endif