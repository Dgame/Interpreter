#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>

template <typename T>
void writeln(const T& val) {
    std::cout << val << std::endl;
}

template <typename T, typename ...Args>
void writeln(const T& val, const Args ...args) {
    std::cout << val;
    writeln(args...);
}

template <typename ...Args>
void assert(bool cond, const Args ...args) {
    if (!cond) {
        writeln(args...);

        throw "An error occured";
    }
}

#endif