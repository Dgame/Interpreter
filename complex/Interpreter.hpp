#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <ostream>
#include <memory>
#include <vector>
#include <string>

#include "Location.hpp"
#include "Declaration.hpp"
#include "Token.hpp"

struct Expr;

class Interpreter {
private:
    Loc _loc;

    std::vector<std::unique_ptr<VarDecl>> _vars;

public:
    bool accept(char);
    bool accept(Tok);

    bool expect(char);
    bool expect(Tok);

    void skipSpaces();
    void skipComment();

    VarDecl* findVar(const std::string&);

    Token readIdentifier();
    Token readNumber();

    void parse(const std::string&);
    void parseVar();
    void parseVarAssign();

    Decl* parsePrint();
    Expr* parseNumber();
    Expr* parseExpr();
    Expr* parseTerm();
    Expr* parseFactor();
};

#endif