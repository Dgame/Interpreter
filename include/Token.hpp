//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_TOKEN_HPP
#define INTERPRETER_TOKEN_HPP

#include <string>

#include "Cursor.hpp"
#include "Tok.hpp"
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

    static const std::string& AsString(Tok);
    static Tok IsKeyword(const std::string&);
};

#endif //INTERPRETER_TOKEN_HPP
