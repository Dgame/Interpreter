#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <ostream>
#include <string>
#include "Value.hpp"

struct Variable {
    std::string name;
    bool isConst = false;
    Value val;

    Variable() = default;
    explicit Variable(const std::string&, bool constant = false);
};

std::ostream& operator <<(std::ostream&, Variable&);

#endif