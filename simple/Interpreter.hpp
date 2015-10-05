#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <map>
#include "Variable.hpp"
#include "Cursor.hpp"

struct Interpreter {
    Cursor cursor;
    std::map<std::string, Variable> vars;

    void makeVar(bool);
    void interpret(const std::string&);
};

#endif