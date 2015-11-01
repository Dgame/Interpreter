//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_LOCATION_HPP
#define INTERPRETER_LOCATION_HPP

#include "Cursor.hpp"

class Location {
public:
    Cursor cursor;

private:
    const char* _first = nullptr;
    const char* _last = nullptr;
    const char* _pos = nullptr;
    const char* _tracker = nullptr;

public:
             Location() = default;
    explicit Location(const char*, const char*);

    void track() {
        _tracker = _pos;
    }

    void backtrack() {
        _pos = _tracker;
    }

    const char* seek() const {
        return _pos;
    }

    void tell(const char* pos) {
        _pos = pos;
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
        return _pos > _last || *_pos == '\0';
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

#endif //INTERPRETER_LOCATION_HPP
