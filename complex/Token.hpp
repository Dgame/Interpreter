#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include "Tok.hpp"
#include "Cursor.hpp"
#include "types.hpp"

struct Token {
    Cursor cursor;
    Tok type;

    union {
        f32_t decimal;
        i32_t integer;
        bool boolean;
        char character;
    };

    std::string identifier;

    Token();
    explicit Token(const Cursor&, Tok);
    explicit Token(const Cursor&, Tok, const std::string&);

    static Token Identify(const Cursor&, const std::string&);
};

#endif