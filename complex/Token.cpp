#include "Token.hpp"

Token::Token(Tok tok) : type(tok) { }
Token::Token(Tok tok, const std::string& str) : type(tok), identifier(str) { }

Token Token::Identify(const std::string& str) {
    if (str.empty()) {
        return Token(Tok::None);
    }

    if (str == "print") {
        return Token(Tok::Output, str);
    }

    if (str == "var") {
        return Token(Tok::Mutable, str);
    }

    if (str == "let") {
        return Token(Tok::Immutable, str);
    }

    return Token(Tok::Identifier, str);
}