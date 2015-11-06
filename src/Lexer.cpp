//
// Created by Randy on 31.10.2015.
//

#include "Lexer.hpp"
#include "Token.hpp"
#include "error.hpp"

#include <fstream>
#include <sstream>

Lexer::Lexer() : _current(new Token()) { }

void Lexer::load(const std::string& filename) {
    std::ifstream stream(filename);
    if (stream.good()) {
        std::ostringstream contents;
        contents << stream.rdbuf();
        stream.close();

        _buffer = contents.str();
        _loc    = Location(&_buffer.front(), &_buffer.back());

        /*
         * Ignore possible header
         */
        while (!_loc.eof() && !_loc.isAlphaNumeric()) {
            _loc.next();
        }
    } else {
        throw "Invalid File: " + filename;
    }
}

bool Lexer::accept(char c) {
    while (!_loc.eof() && _loc.isSpace()) {
        _loc.next();
    }

    if (_loc.getCurrent() == c) {
        _loc.next();

        return true;
    }

    return false;
}

bool Lexer::accept(Tok type) {
    _loc.track();

    const Token* tok = this->read();
    if (tok->type != type) {
        _loc.backtrack();

        return false;
    }

    return true;
}

bool Lexer::expect(char c) {
    if (!this->accept(c)) {
        error("Expected ", c, " @ ", _loc.cursor.lineNr);

        return false;
    }

    return true;
}

bool Lexer::expect(Tok type) {
    if (!this->accept(type)) {
        error("Expected Tok ", Token::AsString(type), " @ ", _loc.cursor.lineNr);

        return false;
    }

    return true;
}

void Lexer::scan(Token* tok) {
    tok->type = Tok::None;

    while (true) {
        switch (_loc.getCurrent()) {
            case 0:
            case 0x1A:
                tok->type = Tok::Eof;
                break;
            case ' ':
            case '\t':
            case '\v':
            case '\f':
                _loc.next();
                continue;
            case '\r':
                _loc.next();
                if (_loc.getCurrent() != '\n') // if CR stands by itself
                    _loc.cursor.lineNr++;
                continue;
            case '\n':
                _loc.next();
                continue;
            case '+':
                if (_loc.peek() == '+')
                    error("Increment (++) is not supported");

                if (_loc.peek() == '=')
                    error("+= is not supported");

                _loc.next();

                tok->type = Tok::Plus;
                break;
            case '-':
                if (_loc.peek() == '-')
                    error("Decrement (--) is not supported");

                if (_loc.peek() == '=')
                    error("-= is not supported");

                _loc.next();

                tok->type = Tok::Minus;
            case '*':
                if (_loc.peek() == '*')
                    error("Pow (**) is not supported");

                if (_loc.peek() == '=')
                    error("*= is not supported");

                _loc.next();

                tok->type = Tok::Mul;
                break;
            case '/':
                if (_loc.peek() == '/')
                    error("C-comments (//) are not supported");

                if (_loc.peek() == '*')
                    error("C-comments (/*) are not supported");

                if (_loc.peek() == '=')
                    error("/= is not supported");

                _loc.next();

                tok->type = Tok::Div;
                break;
            case '%':
                if (_loc.peek() == '%')
                    error("%% is not supported");

                if (_loc.peek() == '=')
                    error("%= is not supported");

                _loc.next();

                tok->type = Tok::Mod;
                break;
            case '(':
                _loc.next();

                tok->type = Tok::OpenParen;
                break;
            case ')':
                _loc.next();

                tok->type = Tok::CloseParen;
                break;
            case '[':
                _loc.next();

                tok->type = Tok::OpenBracket;
                break;
            case ']':
                _loc.next();

                tok->type = Tok::CloseBracket;
                break;
            case '{':
                _loc.next();

                tok->type = Tok::OpenCurly;
                break;
            case '}':
                _loc.next();

                tok->type = Tok::CloseCurly;
                break;
            case ',':
                _loc.next();

                tok->type = Tok::Comma;
                break;
            case '.':
                _loc.next();

                tok->type = Tok::Dot;
                break;
            case ':':
                _loc.next();

                tok->type = Tok::Colon;
                break;
            case ';':
                _loc.next();

                tok->type = Tok::Semicolon;
                break;
            case '=':
                _loc.next();

                if (_loc.getCurrent() == '=') {
                    _loc.next();

                    tok->type = Tok::Equal;
                } else {
                    tok->type = Tok::Assign;
                }
                break;
            case '!':
                _loc.next();

                if (_loc.getCurrent() == '=') {
                    _loc.next();

                    tok->type = Tok::NotEqual;
                } else {
                    tok->type = Tok::Not;
                }
                break;
            case '&':
                _loc.next();

                if (_loc.peek() == '=')
                    error("&= is not supported");

                if (_loc.getCurrent() == '&') {
                    _loc.next();

                    tok->type = Tok::LogicAnd;
                } else {
                    tok->type = Tok::BitAnd;
                }
                break;
            case '|':
                _loc.next();

                if (_loc.peek() == '=')
                     error("|= is not supported");

                if (_loc.getCurrent() == '|') {
                    _loc.next();

                    tok->type = Tok::LogicOr;
                } else {
                    tok->type = Tok::BitOr;
                }
                break;
            case '^':
                if (_loc.peek() == '=')
                    error("^= is not supported");

                _loc.next();

                tok->type = Tok::BitXor;
                break;
            case '~':
                if (_loc.peek() == '=')
                    error("~= is not supported");

                _loc.next();

                tok->type = Tok::BitNot;
                break;
            case '>':
                _loc.next();

                if (_loc.getCurrent() == '=') {
                    _loc.next();

                    tok->type = Tok::GreaterEqual;
                } else {
                    tok->type = Tok::Greater;
                }
                break;
            case '<':
                _loc.next();

                if (_loc.getCurrent() == '=') {
                    _loc.next();

                    tok->type = Tok::LowerEqual;
                } else {
                    tok->type = Tok::Lower;
                }
                break;
            case '\'':
                this->readCharacter(tok);
                break;
            case '"':
                this->readString(tok);
                break;
            case '#':
                _loc.next();

                while (!_loc.eof()) {
                    switch (_loc.getCurrent()) {
                        case 0:
                        case 0x1A:
                        case '\n':
                            break;
                        case '\r':
                            if (_loc.peek() == '\n')
                                _loc.next();
                            else
                                _loc.cursor.lineNr++;
                            break;
                        default:
                            _loc.next();
                            continue;
                    }

                    break;
                }

                continue;
            default:
                if (_loc.isAlpha() || _loc.getCurrent() == '_')
                    this->readIdentifier(tok);
                else if (_loc.isDigit() || (_loc.getCurrent() == '-' && _loc.isNextDigit()))
                    this->readNumber(tok);
                else
                    tok->type = Tok::Eof;

                break;
        }

        break;
    }

    if (tok->type != Tok::None)
        tok->cursor = _loc.cursor;
}

void Lexer::readIdentifier(Token* tok) {
    if (!_loc.isAlpha())
        error("Expected identifier, not ", _loc.getCurrent(), " @ ", _loc.cursor.lineNr);

    std::string str;
    str.reserve(32);

    while (_loc.isAlphaNumeric()) {
        str += _loc.getCurrent();
        _loc.next();
    }

    Token::Identify(str, tok);
}

void Lexer::readNumber(Token* tok) {
    const bool isNegative = this->accept('-');

    if (!_loc.isDigit())
        error("Expected number, not ", _loc.getCurrent(), " @ ", _loc.cursor.lineNr);

    tok->type = Tok::Integer;

    i32_t num = 0;
    do {
        num *= 10;
        num += _loc.getCurrent() - '0';

        _loc.next();
    } while (!_loc.eof() && _loc.isDigit());

    if (this->accept('.')) {
        tok->type = Tok::Decimal;

        f32_t pot = 1;
        i32_t dec = 0;
        do {
            pot *= 10;
            dec *= 10;
            dec += _loc.getCurrent() - '0';

            _loc.next();
        } while (!_loc.eof() && _loc.isDigit());

        tok->decimal = num + (dec / pot);

        if (isNegative)
            tok->decimal *= -1;
    } else {
        tok->integer = num;

        if (isNegative)
            tok->integer *= -1;
    }
}

void Lexer::readCharacter(Token* tok) {
    this->expect('\'');
    const char c = _loc.getCurrent();
    _loc.next();
    this->expect('\'');

    tok->type      = Tok::Character;
    tok->character = c;
}

void Lexer::readString(Token* tok) {
    this->expect('"');

    std::string str;
    str.reserve(32);

    while (!_loc.eof() && _loc.getCurrent() != '"') {
        str += _loc.getCurrent();
        _loc.next();
    }

    this->expect('"');

    tok->type       = Tok::String;
    tok->identifier = str;
}

Token* Lexer::peek(Token* tpl) {
    if (tpl->next) {
        return tpl->next.get();
    }

    Token* tok = new Token();
    this->scan(tok);
    tpl->next.reset(tok);

    return tok;
}

Token* Lexer::read() {
    if (_current->next)
        _current = std::move(_current->next);
    else
        this->scan(_current.get());

    return _current.get();
}

Token* Lexer::peek() {
    return this->peek(_current.get());
}
