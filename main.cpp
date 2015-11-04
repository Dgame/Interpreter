#include <iostream>
#include "Interpreter.hpp"
#include "Expression.hpp"

std::string getDirectory() {
    const std::string file(__FILE__);

    u32_t ds = 0;

    for (u32_t i = file.size() - 1; i > 0; i--) {
        if (file[i] == '\\') {
            ds = i;
            break;
        }
    }

    return file.substr(0, ds + 1);
}

int main() {
    try {
        std::string path = getDirectory();
        path.append("syntax/test.rs");

        Interpreter ip(path);
        ip.parse();
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    } catch (const std::string& str) {
        std::cerr << str << std::endl;
    }
}