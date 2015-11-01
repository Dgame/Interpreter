#include <iostream>
#include "Interpreter.hpp"
#include "Expression.hpp"

int main() {
    Interpreter ip;

    try {
        ip.parse("C:/Users/Randy/Documents/GitHub/Interpreter/syntax/test.rs");
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    } catch (const std::string& str) {
        std::cerr << str << std::endl;
    }
}