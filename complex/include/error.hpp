#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>

// #include "Location.hpp"

template <typename T>
void error(const T& last) {
    std::cerr << last << std::endl;

    throw "Aborting...";
}

template <typename T, typename... Args>
void error(const T& first, Args ...args) {
    std::cerr << "Error: " << first;
    error(args...);
}

template <typename T>
void debug(const T& last) {
    std::cout << last << std::endl;
}

template <typename T, typename... Args>
void debug(const T& first, Args ...args) {
    std::cout << first;
    debug(args...);
}

template <typename T>
void warning(const T& last) {
    std::cerr << last << std::endl;
}

template <typename T, typename... Args>
void warning(const T& first, Args ...args) {
    std::cerr << "Warning: " << first;
    warning(args...);
}

#endif