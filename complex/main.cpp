#include <ostream>
#include "ExprParser.hpp"
#include "Expression.hpp"

int main() {
    ExprParser ep;

    try {
        ep.parse("test.txt");
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }
}