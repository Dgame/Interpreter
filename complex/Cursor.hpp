#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "types.hpp"

struct Cursor {
    u32_t lineNr = 1;
    u32_t column = 1;
};

#endif