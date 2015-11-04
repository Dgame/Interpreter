//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_LEXER_HPP
#define INTERPRETER_LEXER_HPP

#include <string>
#include <stack>
#include <array>

#include "Location.hpp"
#include "Token.hpp"

class Lexer {
private:
    Location _loc;
    std::string _buffer;

    std::stack<const char*> _peeks;

    bool accept(char);
    bool accept(Tok);
    bool expect(char);
    bool expect(Tok);

    Token readIdentifier();
    Token readNumber();
    Token readCharacter();
    Token readString();

public:
    void load(const std::string&);

    Token read();
    Token peek();
    void confirm();
    Token reread();
};

#endif //INTERPRETER_LEXER_HPP
