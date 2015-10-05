#include "Token.hpp"

Token::Token() : type(Type::None) { }
Token::Token(f64_t dval) : type(Type::Decimal), decimal(dval) {  }
Token::Token(i32_t ival) : type(Type::Integer), integer(ival) {  }
Token::Token(const std::string& str) : type(Type::String), id(str.c_str()) {  }
Token::Token(char cval) : type(Type::Character), character(cval) {  }
Token::Token(bool bval) : type(Type::Boolean), boolean(bval) {  }