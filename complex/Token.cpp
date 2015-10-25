#include "Token.hpp"

Token::Token() : type(Tok::None) { }
Token::Token(const Cursor& cur, Tok tok) : cursor(cur), type(tok) { }
Token::Token(const Cursor& cur, Tok tok, const std::string& str) : cursor(cur), type(tok), identifier(str) { }

Token Token::Identify(const Cursor& cur, const std::string& str) {
    if (str.empty()) {
        return Token(cur, Tok::None);
    }

    if (str == "print") {
        return Token(cur, Tok::Output, str);
    }

    if (str == "var") {
        return Token(cur, Tok::Mutable, str);
    }

    if (str == "let") {
        return Token(cur, Tok::Immutable, str);
    }

    return Token(cur, Tok::Identifier, str);
}