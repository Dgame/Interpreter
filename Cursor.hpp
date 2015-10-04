#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "types.hpp"
#include "Token.hpp"

struct Cursor {
    const char* ptr = nullptr;
    u32_t pos = 0;

    Token token;

    Cursor() = default;
    explicit Cursor(const char*);

    char current() const {
        return *(this->ptr + this->pos);
    }

    char next() const {
        return *(this->ptr + this->pos + 1);
    }

    void move() {
        this->pos += 1;
    }

    bool match(const std::string&);
    void ignore();

    Token::Type isNumeric();
    Token::Type isIdentifier();

    Token::Type getToken();
    Token::Type expect(Token::Type);
};

#endif