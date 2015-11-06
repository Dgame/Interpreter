//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_TOKEN_HPP
#define INTERPRETER_TOKEN_HPP

#include <string>
#include <memory>

#include "Cursor.hpp"
#include "Tok.hpp"
#include "types.hpp"

struct Token {
    Cursor cursor;
    Tok    type = Tok::None;

    std::unique_ptr<Token> next;

    union {
        f32_t decimal;
        i32_t integer;
        char  character;
    };

    std::string identifier;

    Token() = default;

    static void Identify(const std::string&, Token*);
    static const std::string& AsString(Tok);
    static Tok IsKeyword(const std::string&);
};

#endif //INTERPRETER_TOKEN_HPP
