#include "Location.hpp"
#include <locale>

Location::Location(const char* start, const char* end) : first(start), last(end), pos(start), tracker(start) { }

/*
* Returns the current character, if any
*/
char Location::getCurrent() const {
    if (this->eof())
        return '\0';

    return *(this->pos);
}

char Location::peek() const {
    if (this->eof())
        return '\0';

    return *(this->pos + 1);
}

/*
* Move to the next char: 'current' will now return the next character
*/
void Location::next() {
    if (this->eof())
        return;

    if (this->getCurrent() == '\n')
        this->cursor.lineNr++;

    this->pos++;
}

bool Lexer::isDigit() const {
    const char c = this->getCurrent();

    return std::isidigt(c);
}

bool Lexer::isSpace() const {
    const char c = this->getCurrent();

    return std::isspace(c);
}

bool Lexer::isAlpha() const {
    const char c = this->getCurrent();

    return std::isalpha(c);
}

bool Lexer::isAlphaNumeric() const {
    const char c = this->getCurrent();

    return std::isalnum(c);
}

bool Lexer::isNextDigit() const {
    const char c = this->peek();

    return std::isidigt(c);
}

bool Lexer::isNextSpace() const {
    const char c = this->peek();

    return std::isspace(c);
}

bool Lexer::isNextAlpha() const {
    const char c = this->peek();

    return std::isalpha(c);
}

bool Lexer::isNextAlphaNumeric() const {
    const char c = this->peek();

    return std::isalnum(c);
}