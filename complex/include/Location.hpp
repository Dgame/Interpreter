#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "types.hpp"
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

    u32_t numOfReadChars() const {
        return _pos - _first;
    }

    u32_t numOfAllChars() const {
        return _last -_first;
    }

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

#endif