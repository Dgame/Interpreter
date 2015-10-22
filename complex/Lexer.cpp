#include "Lexer.hpp"
// #include "Tok.hpp"
#include "types.hpp"
#include "error.hpp"

#include <fstream>
#include <sstream>
#include <locale>

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
        while (!_loc.eof() && !std::isalnum(_loc.getCurrent())) {
            _loc.next();
        }
    } else {
        throw "Invalid File: " + filename;
    }
}

void Lexer::skipSpaces() {
    while (!_loc.eof() && std::isspace(_loc.getCurrent())) {
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
        error("Expected ", c, " @ ", _loc.lineNr);

        return false;
    }

    return true;
}

bool Lexer::expect(Tok type) {
    if (!this->accept(type)) {
        error("Expected Tok ", static_cast<i32_t>(type), " @ ", _loc.lineNr);

        return false;
    }

    return true;
}

Token Lexer::read() {
    while (!_loc.eof()) {
        switch (_loc.getCurrent()) {
            case 0:
            case 0x1A:
                return Token(Tok::Eof);
            case ' ':
            case '\t':
            case '\v':
            case '\f':
                _loc.next();
                continue;
            case '\r':
                _loc.next();
                if (_loc.getCurrent() != '\n') // if CR stands by itself
                    _loc.lineNr++;
                continue;
            case '\n':
                _loc.next();
                continue;
            case '+':
                _loc.next();
                return Token(Tok::Plus);
            case '-':
                _loc.next();
                return Token(Tok::Minus);
            case '*':
                _loc.next();
                return Token(Tok::Mul);
            case '/':
                _loc.next();
                return Token(Tok::Div);
            case '%':
                _loc.next();
                return Token(Tok::Mod);
            case '(':
                _loc.next();
                return Token(Tok::LeftParen);
            case ')':
                _loc.next();
                return Token(Tok::RightParen);
            case '[':
                _loc.next();
                return Token(Tok::LeftBracket);
            case ']':
                _loc.next();
                return Token(Tok::RightBracket);
            case '{':
                _loc.next();
                return Token(Tok::LeftCurly);
            case '}':
                _loc.next();
                return Token(Tok::RightCurly);
            case ',':
                _loc.next();
                return Token(Tok::Comma);
            case '.':
                _loc.next();
                return Token(Tok::Dot);
            case ':':
                _loc.next();
                return Token(Tok::Colon);
            case ';':
                _loc.next();
                return Token(Tok::Semicolon);
            case '=':
                _loc.next();
                if (_loc.getCurrent() == '=') {
                    _loc.next();
                    return Token(Tok::Equal);
                }

                return Token(Tok::Assign);
            case '!':
                _loc.next();
                if (_loc.getCurrent() == '=') {
                    _loc.next();
                    return Token(Tok::NotEqual);
                }

                return Token(Tok::Not);
            case '&':
                _loc.next();
                if (_loc.getCurrent() == '&') {
                    _loc.next();
                    return Token(Tok::LogicAnd);
                }

                return Token(Tok::BitAnd);
            case '|':
                _loc.next();
                if (_loc.getCurrent() == '|') {
                    _loc.next();
                    return Token(Tok::LogicOr);
                }

                return Token(Tok::BitOr);
            case '^':
                _loc.next();
                return Token(Tok::BitXor);
            case '~':
                _loc.next();
                return Token(Tok::BitNot);
            case '>':
                _loc.next();
                if (_loc.getCurrent() == '=') {
                    _loc.next();
                    return Token(Tok::GreaterEqual);
                }

                return Token(Tok::Greater);
            case '<':
                _loc.next();
                if (_loc.getCurrent() == '=') {
                    _loc.next();
                    return Token(Tok::LowerEqual);
                }

                return Token(Tok::Lower);
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
                                _loc.lineNr++;
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
                const char c = _loc.getCurrent();

                if (std::isalpha(c) || c == '_') {
                    return this->readIdentifier();
                }

                if (std::isdigit(c) || (c == '-' && std::isdigit(_loc.peek()))) {
                    return this->readNumber();
                }

                return Token(Tok::Eof);
            }
        }
    }

    return Token(Tok::Eof);
}

Token Lexer::readIdentifier() {
    if (!std::isalpha(_loc.getCurrent())) {
        error("Expected identifier, not ", _loc.getCurrent(), " @ ", _loc.lineNr);

        return Token(Tok::None);
    }

    std::string str;
    str.reserve(32);

    while (std::isalnum(_loc.getCurrent())) {
        str += _loc.getCurrent();
        _loc.next();
    }

    return Token::Identify(str);
}

Token Lexer::readNumber() {
    const bool isNegative = this->accept('-');

    if (!std::isdigit(_loc.getCurrent()))
        return Token(Tok::None);

    Token tok(Tok::Integer);

    i32_t num = 0;
    do {
        num *= 10;
        num += _loc.getCurrent() - '0';

        _loc.next();
    } while (!_loc.eof() && std::isdigit(_loc.getCurrent()));

    if (this->accept('.')) {
        tok.type = Tok::Decimal;

        f32_t pot = 1;
        i32_t dec = 0;
        do {
            pot *= 10;
            dec *= 10;
            dec += _loc.getCurrent() - '0';

            _loc.next();
        } while (!_loc.eof() && std::isdigit(_loc.getCurrent()));

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
    _loc.next();

    if (_loc.getCurrent() != '\'')
        error("Invalid character (too wide)");

    Token tok(Tok::Character);
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

    _loc.next();

    return Token(Tok::String, str);
}

Token Lexer::peek() {
    _loc.track();
    const Token tok = this->read();
    _loc.backtrack();

    return tok;
}