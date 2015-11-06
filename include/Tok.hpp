//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_TOK_HPP
#define INTERPRETER_TOK_HPP

enum class Tok {
    None,
    Eof,
    Integer,
    Decimal,
    Character,
    String,
    Identifier,
    Mutable,
    Immutable,
    Output,
    Null,
    True,
    False,
    If,
    Else,
    For,
    While,
    Plus,
    Minus,
    Mul,
    Div,
    Mod,
    OpenParen,
    CloseParen,
    OpenBracket,
    CloseBracket,
    OpenCurly,
    CloseCurly,
    Comma,
    Dot,
    Colon,
    Semicolon,
    Assign,
    Equal,
    Not,
    NotEqual,
    LogicAnd,
    BitAnd,
    LogicOr,
    BitOr,
    BitXor,
    BitNot,
    Greater,
    GreaterEqual,
    Lower,
    LowerEqual,
};

#endif //INTERPRETER_TOK_HPP
