#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "types.hpp"

struct Location {
    u32_t lineNr = 0;

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
    char getCurrent() const {
        if (this->eof())
            return '\0';
        return *(this->pos);
    }

    /*
    * Move to the next char: 'current' will now return the next character
    */
    void next() {
        if (this->eof())
            return;
        if (this->getCurrent() == '\n')
            this->lineNr++;
        this->pos++;
    }

    /*
    * True if we reached the end of file.
    */
    bool eof() const {
        return this->pos > this->last || *this->pos == '\0';
    }
};

#endif