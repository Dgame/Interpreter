#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "types.hpp"
#include "Cursor.hpp"

struct Location {
    Cursor cursor;

    const char* first = nullptr;
    const char* last = nullptr;
    const char* pos = nullptr;
    const char* tracker = nullptr;

    Location() = default;
    explicit Location(const char*, const char*);

    u32_t numOfReadChars() const {
        return this->pos - this->first;
    }

    u32_t numOfAllChars() const {
        return this->last - this->first;
    }

    void track() {
        this->tracker = this->pos;
    }

    void backtrack() {
        this->pos = this->tracker;
    }

    /*
    * Returns the current character, if any
    */
    char getCurrent() const;
    char peek() const;

    /*
    * Move to the next char: 'current' will now return the next character
    */
    void next();

    /*
    * True if we reached the end of file.
    */
    bool eof() const {
        return this->pos > this->last || *this->pos == '\0';
    }

    bool isDigit() const;
    bool isSpace() const;
    bool isAlpha() const;
    bool isAlphaNumeric() const;

    bool isNextDigit() const;
    bool isNextSpace() const;
    bool isNextAlpha() const;
    bool isNextAlphaNumeric() const;
};

#endif