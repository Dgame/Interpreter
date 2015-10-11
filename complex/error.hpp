#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>

#include "Location.hpp"

template <typename T>
void error(const Location&, const T& last) {
    std::cerr << last << std::endl;

    throw "An error occured";
}

template <typename T, typename... Args>
void error(const Location& loc, const T& first, Args... args) {
    std::cerr << '@' << loc.lineNr << ':' << first;
    error(loc, args...);
}

#endif