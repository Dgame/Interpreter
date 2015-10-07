#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include "types.hpp"

enum class Tok {
    None,
    Integer,
    Decimal,
    Boolean,
    Character,
    String,
    Identifier,
    Mutable,
    Immutable,
    Output
};

struct Token {
    Tok type;

    union {
        f32_t decimal;
        i32_t integer;
        bool boolean;
        char character;
    };

    std::string identifier;

    explicit Token(Tok);
    explicit Token(Tok, const std::string&);

    static Token Identify(const std::string&);
};

#endif