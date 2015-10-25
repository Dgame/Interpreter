#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <ostream>
#include <memory>
#include <string>

#include "Declaration.hpp"
#include "Scope.hpp"
#include "Lexer.hpp"

struct Expr;

class Interpreter {
private:
    Lexer _lex;
    std::unique_ptr<Scope> _scope;

    bool accept(Tok);
    void expect(Tok, u32_t);

    void pushScope();
    void popScope();

public:
    void parse(const std::string&);

private:
    bool parseVar();
    bool parseVarAssign();
    bool parsePrint();

    Expr* parseArrayExpr();
    Expr* parseNumericExpr();
    Expr* parseIndexOfExpr(const VarDecl*);
    Expr* parseExpr();
    Expr* parseTerm();
    Expr* parseFactor();
    Expr* parseVariableFactor();
};

#endif