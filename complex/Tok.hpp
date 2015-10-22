#ifndef TOK_HPP
#define TOK_HPP

enum class Tok {
    None,
    Eof,
    Integer,
    Decimal,
    Boolean,
    Character,
    String,
    Identifier,
    Mutable,
    Immutable,
    Output,
    Plus,
    Minus,
    Mul,
    Div,
    Mod,
    LeftParen,
    RightParen,
    LeftBracket,
    RightBracket,
    LeftCurly,
    RightCurly,
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

#endif