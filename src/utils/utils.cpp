#include "utils.h"

void print(std::string string) {
    std::cout << string << '\n';
}

Mixed input(std::string string) {
    std::string value;
    Mixed result;
    std::cout << string;
    std::cin >> value;
    result = Mixed(value);
    return result;
}
