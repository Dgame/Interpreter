//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_CURSOR_HPP
#define INTERPRETER_CURSOR_HPP

#include "types.hpp"

struct Cursor {
    u32_t lineNr = 1;
    u32_t column = 1;
};

#endif //INTERPRETER_CURSOR_HPP
