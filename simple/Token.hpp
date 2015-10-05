#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>
#include <map>

#include "types.hpp"
#include "error.hpp"

struct Token {
    enum Type {
        None,
        Eof,
        Constant,
        Variable,
        Output,
        Identifier,
        Integer,
        Decimal,
        String,
        Character,
        Null,
        Boolean,
        Comma,
        Assign,
        Plus,
        Minus,
        Mul,
        Div,
        Mod,
        StmtEnd,
    };

    Type type;

    union {
        f64_t decimal;
        i32_t integer;
        const char* id;
        char character;
        bool boolean;
    };

    Token();
    Token(f64_t);
    Token(i32_t);
    Token(const std::string&);
    Token(char);
    Token(bool);
};

const std::map<std::string, Token::Type> Plural = {
    {"let", Token::Constant},
    {"var", Token::Variable},
    {"print", Token::Output},
    {"null", Token::Null},
};

const std::map<char, Token::Type> Singular = {
    {'=', Token::Assign},
    {'+', Token::Plus},
    {'-', Token::Minus},
    {'*', Token::Mul},
    {'/', Token::Div},
    {'%', Token::Mod},
    {',', Token::Comma},
    {';', Token::StmtEnd},
};

#endif