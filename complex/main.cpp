#include <ostream>
#include "Interpreter.hpp"
#include "Expression.hpp"

int main() {
    Interpreter ip;

    try {
        ip.parse("test.txt");
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }
}