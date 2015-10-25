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
    Location _loc;
    Lexer _lex;
    std::unique_ptr<Scope> _scope;

    bool accept(Tok, Token* the_tok = nullptr);
    bool expect(Tok);

    void pushScope();
    void popScope();

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