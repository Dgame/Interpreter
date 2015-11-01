//
// Created by Randy on 31.10.2015.
//

#include "Token.hpp"
#include <map>

namespace {
    std::map<Tok, std::string> Tokens = {
        {Tok::None, "None"},
        {Tok::Eof, "EOF"},
        {Tok::Integer, "Integer"},
        {Tok::Decimal, "Decimal"},
        {Tok::Boolean, "Boolean"},
        {Tok::Character, "Character"},
        {Tok::String, "String"},
        {Tok::Identifier, "Identifier"},
        {Tok::Mutable, "var"},
        {Tok::Immutable, "let"},
        {Tok::Output, "print"},
        {Tok::Plus, "+"},
        {Tok::Minus, "-"},
        {Tok::Mul, "*"},
        {Tok::Div, "/"},
        {Tok::Mod, "%"},
        {Tok::OpenParen, "("},
        {Tok::CloseParen, ")"},
        {Tok::OpenBracket, "["},
        {Tok::CloseBracket, "]"},
        {Tok::OpenCurly, "{"},
        {Tok::CloseCurly, "}"},
        {Tok::Comma, ","},
        {Tok::Dot, "."},
        {Tok::Colon, ":"},
        {Tok::Semicolon, ";"},
        {Tok::Assign, "="},
        {Tok::Equal, "=="},
        {Tok::Not, "!"},
        {Tok::NotEqual, "!="},
        {Tok::LogicAnd, "&&"},
        {Tok::BitAnd, "&"},
        {Tok::LogicOr, "||"},
        {Tok::BitOr, "|"},
        {Tok::BitXor, "^"},
        {Tok::BitNot, "~"},
        {Tok::Greater, ">"},
        {Tok::GreaterEqual, ">="},
        {Tok::Lower, "<"},
        {Tok::LowerEqual, "<="},
    };
}

Token::Token() : type(Tok::None) { }
Token::Token(const Cursor& cur, Tok tok) : cursor(cur), type(tok) { }
Token::Token(const Cursor& cur, Tok tok, const std::string& str) : cursor(cur), type(tok), identifier(str) { }

Token Token::Identify(const Cursor& cur, const std::string& str) {
    if (str.empty()) {
        return Token(cur, Tok::None);
    }

    const Tok tok = Token::IsKeyword(str);
    if (tok != Tok::None) {
        return Token(cur, tok, str);
    }

    return Token(cur, Tok::Identifier, str);
}

const std::string& Token::AsString(Tok tok) {
    auto it = Tokens.find(tok);
    if (it != Tokens.end()) {
        return it->second;
    }

    return Token::AsString(Tok::None);
}

Tok Token::IsKeyword(const std::string& str) {
    for (auto& pair : Tokens) {
        if (pair.second == str)
            return pair.first;
    }

    return Tok::None;
}
