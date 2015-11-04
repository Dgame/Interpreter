//
// Created by Randy on 31.10.2015.
//

#include "Lexer.hpp"
#include "error.hpp"

#include <fstream>
#include <sstream>

void Lexer::load(const std::string& filename) {
    std::ifstream stream(filename);
    if (stream.good()) {
        std::ostringstream contents;
        contents << stream.rdbuf();
        stream.close();

        _buffer = contents.str();
        _loc = Location(&_buffer.front(), &_buffer.back());

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

    const Token tok = this->read();
    if (tok.type != type) {
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
        error("Expected Tok ", static_cast<i32_t>(type), " @ ", _loc.cursor.lineNr);

        return false;
    }

    return true;
}

Token Lexer::read() {
    _loc.track(); // track position

    while (!_loc.eof()) {
        switch (_loc.getCurrent()) {
            case 0:
            case 0x1A:
                return Token(_loc.cursor, Tok::Eof);
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

                _loc.next();

                return Token(_loc.cursor, Tok::Plus);
            case '-':
                if (_loc.peek() == '-')
                    error("Decrement (--) is not supported");
                _loc.next();
                return Token(_loc.cursor, Tok::Minus);
            case '*':
                if (_loc.peek() == '*')
                    error("Pow (**) is not supported");
                _loc.next();
                return Token(_loc.cursor, Tok::Mul);
            case '/':
                if (_loc.peek() == '*')
                    error("C-comments (//) are not supported");
                _loc.next();
                return Token(_loc.cursor, Tok::Div);
            case '%':
                if (_loc.peek() == '%')
                    error("%% is not supported");
                _loc.next();
                return Token(_loc.cursor, Tok::Mod);
            case '(':
                _loc.next();
                return Token(_loc.cursor, Tok::OpenParen);
            case ')':
                _loc.next();
                return Token(_loc.cursor, Tok::CloseParen);
            case '[':
                _loc.next();
                return Token(_loc.cursor, Tok::OpenBracket);
            case ']':
                _loc.next();
                return Token(_loc.cursor, Tok::CloseBracket);
            case '{':
                _loc.next();
                return Token(_loc.cursor, Tok::OpenCurly);
            case '}':
                _loc.next();
                return Token(_loc.cursor, Tok::CloseCurly);
            case ',':
                _loc.next();
                return Token(_loc.cursor, Tok::Comma);
            case '.':
                _loc.next();
                return Token(_loc.cursor, Tok::Dot);
            case ':':
                _loc.next();
                return Token(_loc.cursor, Tok::Colon);
            case ';':
                _loc.next();
                return Token(_loc.cursor, Tok::Semicolon);
            case '=':
            {
                _loc.next();

                const char c = _loc.getCurrent();
                switch (c) {
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    case '%':
                    case '&':
                    case '|':
                    case '^':
                        error("operation ", c, "= is not supported.");
                        break;
                    default: break;
                }

                if (c == '=') {
                    _loc.next();
                    return Token(_loc.cursor, Tok::Equal);
                }

                return Token(_loc.cursor, Tok::Assign);
            }
            case '!':
                _loc.next();
                if (_loc.getCurrent() == '=') {
                    _loc.next();
                    return Token(_loc.cursor, Tok::NotEqual);
                }

                return Token(_loc.cursor, Tok::Not);
            case '&':
                _loc.next();
                if (_loc.getCurrent() == '&') {
                    _loc.next();
                    return Token(_loc.cursor, Tok::LogicAnd);
                }

                return Token(_loc.cursor, Tok::BitAnd);
            case '|':
                _loc.next();
                if (_loc.getCurrent() == '|') {
                    _loc.next();
                    return Token(_loc.cursor, Tok::LogicOr);
                }

                return Token(_loc.cursor, Tok::BitOr);
            case '^':
                _loc.next();
                return Token(_loc.cursor, Tok::BitXor);
            case '~':
                _loc.next();
                return Token(_loc.cursor, Tok::BitNot);
            case '>':
                _loc.next();
                if (_loc.getCurrent() == '=') {
                    _loc.next();
                    return Token(_loc.cursor, Tok::GreaterEqual);
                }

                return Token(_loc.cursor, Tok::Greater);
            case '<':
                _loc.next();
                if (_loc.getCurrent() == '=') {
                    _loc.next();
                    return Token(_loc.cursor, Tok::LowerEqual);
                }

                return Token(_loc.cursor, Tok::Lower);
            case '\'':
                return this->readCharacter();
            case '"':
                return this->readString();
            case '#':
            {
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
            }
            default:
            {
                if (_loc.isAlpha() || _loc.getCurrent() == '_')
                    return this->readIdentifier();

                if (_loc.isDigit() || (_loc.getCurrent() == '-' && _loc.isNextDigit())) {
                    return this->readNumber();
                }

                return Token(_loc.cursor, Tok::Eof);
            }
        }
    }

    return Token(_loc.cursor, Tok::Eof);
}

Token Lexer::readIdentifier() {
    if (!_loc.isAlpha()) {
        error("Expected identifier, not ", _loc.getCurrent(), " @ ", _loc.cursor.lineNr);

        return Token(_loc.cursor, Tok::None);
    }

    std::string str;
    str.reserve(32);

    while (_loc.isAlphaNumeric()) {
        str += _loc.getCurrent();
        _loc.next();
    }

    return Token::Identify(_loc.cursor, str);
}

Token Lexer::readNumber() {
    const bool isNegative = this->accept('-');

    if (!_loc.isDigit())
        return Token(_loc.cursor, Tok::None);

    Token tok(_loc.cursor, Tok::Integer);

    i32_t num = 0;
    do {
        num *= 10;
        num += _loc.getCurrent() - '0';

        _loc.next();
    } while (!_loc.eof() && _loc.isDigit());

    if (this->accept('.')) {
        tok.type = Tok::Decimal;

        f32_t pot = 1;
        i32_t dec = 0;
        do {
            pot *= 10;
            dec *= 10;
            dec += _loc.getCurrent() - '0';

            _loc.next();
        } while (!_loc.eof() && _loc.isDigit());

        tok.decimal = num + (dec / pot);

        if (isNegative)
            tok.decimal *= -1;
    } else {
        tok.integer = num;

        if (isNegative)
            tok.integer *= -1;
    }

    return tok;
}

Token Lexer::readCharacter() {
    this->expect('\'');
    const char c = _loc.getCurrent();
    this->expect('\'');

    Token tok(_loc.cursor, Tok::Character);
    tok.character = c;

    return tok;
}

Token Lexer::readString() {
    this->expect('"');

    std::string str;
    str.reserve(32);

    while (!_loc.eof() && _loc.getCurrent() != '"') {
        str += _loc.getCurrent();
        _loc.next();
    }

    this->expect('"');

    return Token(_loc.cursor, Tok::String, str);
}

Token Lexer::peek() {
    _loc.track(); // track position

    const Token tok = this->read();

    _peeks.push(_loc.seek());
    _loc.backtrack(); // go back

    return tok;
}

void Lexer::confirm() {
    const char* pos = _peeks.top();
    _peeks.pop();

    _loc.tell(pos);
}

Token Lexer::reread() {
    _loc.backtrack(); // go to last readed position

    return this->read();
}