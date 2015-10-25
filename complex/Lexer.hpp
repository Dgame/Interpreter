#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <stack>
#include "Location.hpp"
#include "Token.hpp"

class Lexer {
private:
    Location _loc;
    std::string _buffer;

    std::stack<const char*> _peeks;

    void skipSpaces();
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

#endif