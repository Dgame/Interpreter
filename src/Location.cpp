//
// Created by Randy on 31.10.2015.
//

#include "Location.hpp"
#include <locale>

Location::Location(const char* start, const char* end) : _first(start), _last(end), _pos(start), _tracker(start) { }

/*
* Returns the current character, if any
*/
char Location::getCurrent() const {
    if (this->eof())
        return '\0';
    return *(_pos);
}

char Location::peek() const {
    if (this->eof())
        return '\0';
    return *(_pos + 1);
}

/*
* Move to the next char: 'current' will now return the next character
*/
void Location::next() {
    if (this->eof())
        return;

    if (this->getCurrent() == '\n')
        this->cursor.lineNr++;

    _pos++;
}

bool Location::isDigit() const {
    const char c = this->getCurrent();

    return std::isdigit(c);
}

bool Location::isSpace() const {
    const char c = this->getCurrent();

    return std::isspace(c);
}

bool Location::isAlpha() const {
    const char c = this->getCurrent();

    return std::isalpha(c);
}

bool Location::isAlphaNumeric() const {
    const char c = this->getCurrent();

    return std::isalnum(c);
}

bool Location::isNextDigit() const {
    const char c = this->peek();

    return std::isdigit(c);
}

bool Location::isNextSpace() const {
    const char c = this->peek();

    return std::isspace(c);
}

bool Location::isNextAlpha() const {
    const char c = this->peek();

    return std::isalpha(c);
}

bool Location::isNextAlphaNumeric() const {
    const char c = this->peek();

    return std::isalnum(c);
}