//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_LEXER_HPP
#define INTERPRETER_LEXER_HPP

#include <string>
#include <memory>

#include "Location.hpp"

enum class Tok;

struct Token;

class Lexer {
private:
    Location _loc;
    std::string _buffer;
    std::unique_ptr<Token> _current;

    bool accept(char);
    bool accept(Tok);
    bool expect(char);
    bool expect(Tok);

    void scan(Token*);

    void readIdentifier(Token*);
    void readNumber(Token*);
    void readCharacter(Token*);
    void readString(Token*);

    Token* peek(Token*);

public:
    explicit Lexer();

    void load(const std::string&);

    const Token* current() const {
        return _current.get();
    }

    Token* read();
    Token* peek();
};

#endif //INTERPRETER_LEXER_HPP
