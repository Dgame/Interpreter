#include <sstream>
#include "Interpreter.hpp"

int main() {
    std::stringstream ss;
    ss << "let a = 3.14;" << std::endl;
    ss << "var b = 42;" << std::endl;
    ss << "print a, b;" << std::endl;
    // ss << "a = 23;" << std::endl;
    ss << "b = 23;" << std::endl;
    ss << "print a, b;" << std::endl;

    Interpreter ip;
    try {
        ip.interpret(ss.str());
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }
}