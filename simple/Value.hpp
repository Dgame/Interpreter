#ifndef VALUE_HPP
#define VALUE_HPP

#include <ostream>
#include "types.hpp"

enum class Type;
struct Token;

struct Value {
    Type type;

    union {
        f64_t decimal;
        i32_t integer;
        const char* id;
        char character;
        bool boolean;
    };

    Value() = default;
    Value(f64_t);
    Value(i32_t);
    Value(const std::string&);
    Value(char);
    Value(bool);
    Value(const Token&);
};

std::ostream& operator <<(std::ostream&, const Value&);

template <typename T>
Value type_cast(T, Type);

Value cast(const Value&, Type);

#endif