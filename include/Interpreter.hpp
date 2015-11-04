//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_INTERPRETER_HPP
#define INTERPRETER_INTERPRETER_HPP

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <ostream>
#include <memory>
#include <vector>
#include <string>

#include "Declaration.hpp"
#include "Scope.hpp"
#include "Lexer.hpp"

struct Expr;

class Interpreter {
private:
    Lexer                  _lex;
    std::unique_ptr<Scope> _scope;

    bool accept(Tok);
    void expect(Tok, u32_t);

    void pushScope();
    void popScope();

public:
    explicit Interpreter(const std::string&);

    void parse();

private:
    void parseVariable(const Token&);
    void parseAssignment(const Token&);
    void parsePrint(const Token&);

    Expr* parseArrayExpr();
    Expr* parseNumericExpr();
    Expr* parseIndexOfExpr(const VarDecl*);
    Expr* parseExpr();
    Expr* parseTerm();
    Expr* parseFactor();
    Expr* parseVariableFactor();
};

#endif

#endif //INTERPRETER_INTERPRETER_HPP
