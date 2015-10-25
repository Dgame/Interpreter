#include "Lexer.hpp"
#include "types.hpp"
#include "error.hpp"

#include <fstream>
#include <sstream>

Lexer::Lexer(const std::string& filename) {
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

void Lexer::skipSpaces() {
    while (!_loc.eof() && _loc.isSpace()) {
        _loc.next();
    }
}

bool Lexer::accept(char c) {
    this->skipSpaces();

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
                _loc.next();
                return Token(_loc.cursor, Tok::Plus);
            case '-':
                _loc.next();
                return Token(_loc.cursor, Tok::Minus);
            case '*':
                _loc.next();
                return Token(_loc.cursor, Tok::Mul);
            case '/':
                _loc.next();
                return Token(_loc.cursor, Tok::Div);
            case '%':
                _loc.next();
                return Token(_loc.cursor, Tok::Mod);
            case '(':
                _loc.next();
                return Token(_loc.cursor, Tok::LeftParen);
            case ')':
                _loc.next();
                return Token(_loc.cursor, Tok::RightParen);
            case '[':
                _loc.next();
                return Token(_loc.cursor, Tok::LeftBracket);
            case ']':
                _loc.next();
                return Token(_loc.cursor, Tok::RightBracket);
            case '{':
                _loc.next();
                return Token(_loc.cursor, Tok::LeftCurly);
            case '}':
                _loc.next();
                return Token(_loc.cursor, Tok::RightCurly);
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
                _loc.next();
                if (_loc.getCurrent() == '=') {
                    _loc.next();
                    return Token(_loc.cursor, Tok::Equal);
                }

                return Token(_loc.cursor, Tok::Assign);
            case '!':
                _loc.next();
                if (_loc.getCurrent() == '=') {
                    _loc.next();
                    return Token(_loc.cursor, Tok::NotEqual);
                }

                return Token(Tok::Not);
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
        error("Expected identifier, not ", _loc.getCurrent(), " @ ", _loc.lineNr);

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
    _loc.track();
    const Token tok = this->read();
    _loc.backtrack();

    return tok;
}