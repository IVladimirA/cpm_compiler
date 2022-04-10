#ifndef CPM_COMPILER_MIXED_MIXED_H_
#define CPM_COMPILER_MIXED_MIXED_H_

#include <string>

enum DataType {
    dt_int,
    dt_float,
    dt_string,
    dt_undef
};

class Mixed {
public:
    Mixed();
    Mixed(long long int num);
    Mixed(int num);
    Mixed(double num);
    Mixed(const std::string& string);
    bool is_defined() const;
    operator std::string() const;
    friend Mixed to_numeric(const Mixed& m);
    friend Mixed operator+(const Mixed& m1, const Mixed& m2);
    friend Mixed operator-(const Mixed& m1, const Mixed& m2);
private:
    DataType type;
    long long int integer;
    double floating;
    std::string string;
};

// Used as return value when checking string type
enum StringType {
    str_int,
    str_float,
    str_nonnumeric
};

StringType is_numeric(const std::string& s);

void print(const Mixed& m);
Mixed input(const Mixed& m);

#endif