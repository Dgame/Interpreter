#include "Cursor.hpp"

#include <locale>
#include <cstdio>
#include <sstream>

Cursor::Cursor(const char* p) : ptr(p) { }

bool Cursor::match(const std::string& value) {
    const u32_t bt_pos = this->pos;

    for (char c : value) {
        if (c != this->current()) {
            this->pos = bt_pos;

            return false;
        }

        this->move();
    }

    return true;
}

void Cursor::ignore() {
    while (std::isspace(this->current())) {
        this->move();
    }
}

Token::Type Cursor::isNumeric() {
    if (!std::isdigit(this->current())) {
        return Token::None;
    }

    std::string number;
    number.reserve(32);

    while (std::isdigit(this->current())) {
        number += this->current();

        this->move();
    }

    if (this->current() == '.') {
        number += this->current();

        this->move();

        while (std::isdigit(this->current())) {
            number += this->current();

            this->move();
        }

        f64_t dval;
        std::istringstream(number) >> dval;

        this->token = dval;

        return Token::Decimal;
    }

    i32_t ival;
    std::istringstream(number) >> ival;

    this->token = ival;

    return Token::Integer;
}

Token::Type Cursor::isIdentifier() {
    if (!std::isalpha(this->current())) {
        return Token::None;
    }

    std::string str;
    str.reserve(32);

    while (std::isalnum(this->current())) {
        str += this->current();
        this->move();
    }

    this->token = str;

    return Token::Identifier;
}

Token::Type Cursor::getToken() {
    this->ignore();

    for (auto& pair : Plural) {
        if (this->match(pair.first)) {
            return pair.second;
        }
    }

    for (auto& pair : Singular) {
        if (this->current() == pair.first) {
            this->move();

            return pair.second;
        }
    }

    Token::Type tok = this->isIdentifier();
    if (tok != Token::None) {
        return tok;
    }

    tok = this->isNumeric();
    if (tok != Token::None) {
        return tok;
    }

    return Token::Eof;
}

Token::Type Cursor::expect(Token::Type type) {
    const Token::Type tok = this->getToken();
    if (tok != type) {
        error("Unexpected Token: Expected ", type, ", got ", tok);
        throw "Cursor Error";
    }

    return tok;
}