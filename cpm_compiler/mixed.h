#pragma once

#include <string>

enum class DataType {
    INT,
    FLOAT,
    STRING,
    UNDEF
};

class Mixed {
public:
    Mixed() = default;
    Mixed(long long int num);
    Mixed(int num);
    Mixed(double num);
    Mixed(std::string string);
    bool is_defined() const;
    std::string to_string() const;
    Mixed to_numeric() const;
    friend Mixed operator+(const Mixed& m1, const Mixed& m2);
    friend Mixed operator-(const Mixed& m1, const Mixed& m2);
private:
    DataType type = DataType::UNDEF;
    long long int integer = 0;
    double floating = 0.0;
    std::string string;
};

// Used as return value when checking string type
enum class StringType {
    INT,
    FLOAT,
    NONNUMERIC
};

StringType is_numeric(const std::string& s);
void print(const Mixed& m);
Mixed input(const Mixed& m);
