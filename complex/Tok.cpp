#include "Tok.hpp"

#include <string>
#include <map>

namespace {
    std::map<Tok, std::string> Tokens = {
        {None, "None"},
        {Eof, "EOF"},
        {Integer, "Integer"},
        {Decimal, "Decimal"},
        {Boolean, "Boolean"},
        {Character, "Character"},
        {String, "String"},
        {Identifier, "Identifier"}
        {Mutable, "var"},
        {Immutable, "let"},
        {Output, "print"},
        {Plus, "+"},
        {Minus, "-"},
        {Mul, "*"},
        {Div, "/"},
        {Mod, "%"},
        {LeftParen, "("},
        {RightParen, ")"},
        {LeftBracket, "["},
        {RightBracket, "]"},
        {LeftCurly, "{"},
        {RightCurly, "}"},
        {Comma, ","},
        {Dot, "."},
        {Colon, ":"},
        {Semicolon, ";"},
        {Assign, "="},
        {Equal, "=="},
        {Not, "!"},
        {NotEqual, "!="},
        {LogicAnd, "&&"},
        {BitAnd, "&"},
        {LogicOr, "||"},
        {BitOr, "|"},
        {BitXor, "^"},
        {BitNot, "~"},
        {Greater, ">"},
        {GreaterEqual, ">="},
        {Lower, "<"},
        {LowerEqual, "<="},
    };
}

namespace Tok {
    const std::string& AsString(Tok type) {

    }
}