#ifndef LEXER_HP
#define LEXER_HPP

#include <string>
#include "Location.hpp"
#include "Token.hpp"

class Lexer {
private:
    Location _loc;
    std::string _buffer;

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
    explicit Lexer(const std::string&);

    Token read();
    Token peek();
};

#endif