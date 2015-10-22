#include <iostream>
// #include "Interpreter.hpp"
// #include "Expression.hpp"
#include "Lexer.hpp"

int main() {
    // Interpreter ip;

    // try {
    //     ip.parse("test.txt");
    // } catch (const char* msg) {
    //     std::cerr << msg << std::endl;
    // }

    try {
        Lexer lex("test.txt");

        while (true) {
            const Token tok = lex.read();

            switch (tok.type) {
                default:
                    std::cout << "WTF" << std::endl;
                    break;
                case Tok::None:
                    std::cout << "None" << std::endl;
                    break;
                case Tok::Eof:
                    std::cout << "EOF" << std::endl;
                    break;
                case Tok::Integer:
                    std::cout << "integer: " << tok.integer << std::endl;
                    continue;
                case Tok::Decimal:
                    std::cout << "decimal: " << tok.decimal << std::endl;
                    continue;
                case Tok::Boolean:
                    std::cout << "boolean: " << tok.boolean << std::endl;
                    continue;
                case Tok::Character:
                    std::cout << "character: " << tok.character << std::endl;
                    continue;
                case Tok::String:
                    std::cout << "string: " << tok.identifier << std::endl;
                    continue;
                case Tok::Identifier:
                    std::cout << "identifier: " << tok.identifier << std::endl;
                    continue;
                case Tok::Mutable:
                    std::cout << "var" << std::endl;
                    continue;
                case Tok::Immutable:
                    std::cout << "let" << std::endl;
                    continue;
                case Tok::Output:
                    std::cout << "print" << std::endl;
                    continue;
                case Tok::Plus:
                    std::cout << " + " << std::endl;
                    continue;
                case Tok::Minus:
                    std::cout << " - " << std::endl;
                    continue;
                case Tok::Mul:
                    std::cout << " * " << std::endl;
                    continue;
                case Tok::Div:
                    std::cout << " / " << std::endl;
                    continue;
                case Tok::Mod:
                    std::cout << " % " << std::endl;
                    continue;
                case Tok::LeftParen:
                    std::cout << " ( " << std::endl;
                    continue;
                case Tok::RightParen:
                    std::cout << " ) " << std::endl;
                    continue;
                case Tok::LeftBracket:
                    std::cout << " [ " << std::endl;
                    continue;
                case Tok::RightBracket:
                    std::cout << " ] " << std::endl;
                    continue;
                case Tok::LeftCurly:
                    std::cout << " { " << std::endl;
                    continue;
                case Tok::RightCurly:
                    std::cout << " } " << std::endl;
                    continue;
                case Tok::Comma:
                    std::cout << " , " << std::endl;
                    continue;
                case Tok::Dot:
                    std::cout << " . " << std::endl;
                    continue;
                case Tok::Colon:
                    std::cout << " : " << std::endl;
                    continue;
                case Tok::Semicolon:
                    std::cout << " ; " << std::endl;
                    continue;
                case Tok::Assign:
                    std::cout << " = " << std::endl;
                    continue;
                case Tok::Equal:
                    std::cout << " == " << std::endl;
                    continue;
                case Tok::Not:
                    std::cout << " ! " << std::endl;
                    continue;
                case Tok::NotEqual:
                    std::cout << " != " << std::endl;
                    continue;
                case Tok::LogicAnd:
                    std::cout << " && " << std::endl;
                    continue;
                case Tok::BitAnd:
                    std::cout << " & " << std::endl;
                    continue;
                case Tok::LogicOr:
                    std::cout << " || " << std::endl;
                    continue;
                case Tok::BitOr:
                    std::cout << " | " << std::endl;
                    continue;
                case Tok::BitXor:
                    std::cout << " ^ " << std::endl;
                    continue;
                case Tok::BitNot:
                    std::cout << " ~ " << std::endl;
                    continue;
                case Tok::Greater:
                    std::cout << " > " << std::endl;
                    continue;
                case Tok::GreaterEqual:
                    std::cout << " >= " << std::endl;
                    continue;
                case Tok::Lower:
                    std::cout << " < " << std::endl;
                    continue;
                case Tok::LowerEqual:
                    std::cout << " <= " << std::endl;
                    continue;
            }

            break;
        }
     } catch (const std::string& msg) {
        std::cerr << msg << std::endl;
    }
}