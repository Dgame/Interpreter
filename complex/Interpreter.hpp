#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <ostream>
#include <memory>
#include <vector>
#include <string>

#include "Location.hpp"
#include "Declaration.hpp"
#include "Scope.hpp"
#include "Token.hpp"

struct Expr;

class Interpreter {
private:
    Location _loc;
    std::unique_ptr<Scope> _scope;

    void pushScope();
    void popScope();

    bool accept(char);
    bool accept(Tok);

    bool expect(char);
    bool expect(Tok);

    void skipSpaces();
    void skipComment();

    Token readIdentifier();
    Token readNumber();

public:
    void parse(const std::string&);

private:
    void parseVar();
    void parseVarAssign();

    Decl* parsePrint();
    Expr* parseString();
    Expr* parseArray();
    Expr* parseNumber();
    Expr* parseIndexOf(const VarDecl*);
    Expr* parseExpr();
    Expr* parseTerm();
    Expr* parseFactor();
    Expr* parseVariableFactor();
};

#endif