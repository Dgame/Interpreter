//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_ERROR_HPP
#define INTERPRETER_ERROR_HPP

#include <iostream>

template <typename T>
void println(std::ostream& stream, const T& last) {
    stream << last << std::endl;
}

template <typename T, typename... Args>
void println(std::ostream& stream, const T& first, Args&& ...args) {
    stream << first;
    println(stream, args...);
}

template <typename... Args>
void debug(Args&& ...args) {
    println(std::cout, args...);
}

template <typename... Args>
void error(Args&& ...args) {
    std::cerr << "Error: ";
    println(std::cerr, args...);

    throw "Aborting...";
}

template <typename... Args>
void warning(Args&& ...args) {
    std::cerr << "Warning: ";
    println(std::cerr, args...);
}

#endif //INTERPRETER_ERROR_HPP
