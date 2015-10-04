#include "Value.hpp"
#include "match.hpp"
#include "Token.hpp"

#include <cassert>
#include <sstream>

Value::Value(f64_t dval) : type(Type::Decimal), decimal(dval) {  }
Value::Value(i32_t ival) : type(Type::Integer), integer(ival) {  }
Value::Value(const std::string& str) : type(Type::String), id(str.c_str()) {  }
Value::Value(char cval) : type(Type::Character), character(cval) {  }
Value::Value(bool bval) : type(Type::Boolean), boolean(bval) {  }

Value::Value(const Token& token) {
    switch (token.type) {
        default:
        case Token::None:
            this->type = Type::None;
            break;
        case Token::Integer:
            this->type = Type::Integer;
            this->integer = token.integer;
            break;
        case Token::Decimal:
            this->type = Type::Decimal;
            this->decimal = token.decimal;
            break;
        case Token::String:
            this->type = Type::String;
            this->id = token.id;
            break;
        case Token::Character:
            this->type = Type::Character;
            this->character = token.character;
            break;
        case Token::Boolean:
            this->type = Type::Boolean;
            this->boolean = token.boolean;
            break;
    }
}

std::ostream& operator <<(std::ostream& out, const Value& val) {
    switch (val.type) {
        case Type::None:
            out << "NULL";
            break;
        case Type::Integer:
            out << val.integer;
            break;
        case Type::Decimal:
            out << val.decimal;
            break;
        case Type::String:
            out << val.id;
            break;
        case Type::Character:
            out << val.character;
            break;
        case Type::Boolean:
            out << val.boolean;
            break;
    }

    return out;
}

template <typename T>
Value type_cast(T val, Type type) {
    switch (type) {
        default:
        case Type::None:
            return Value();
        case Type::Integer:
            return static_cast<i32_t>(val);
        case Type::Decimal:
            return static_cast<f64_t>(val);
        case Type::Character:
            return static_cast<char>(val);
        case Type::String:
        {
            std::stringstream ss;
            ss << val;

            return ss.str();
        }
    }
}

Value cast(const Value& val, Type type) {
    const Match m = match(val.type, type);

    if (m == Match::Exact)
        return val;

    if (m == Match::No)
        return Value();

    switch (val.type) {
        default:
        case Type::None:
            return Value();
        case Type::Integer:
            return type_cast(val.integer, type);
        case Type::Decimal:
            return type_cast(val.decimal, type);
        case Type::Character:
            return type_cast(val.character, type);
        case Type::String:
            return Value();
    }
}