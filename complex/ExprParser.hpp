#ifndef EXPR_PARSER_HPP
#define EXPR_PARSER_HPP

#include <ostream>
#include <memory>
#include <vector>
#include <string>

#include "Location.hpp"
#include "Declaration.hpp"

struct Expr;

class ExprParser {
private:
    Loc _loc;

    std::vector<std::unique_ptr<VarDecl>> _vars;

public:
    bool accept(char);
    bool accept(const std::string&, bool partial = false);
    bool expect(char);

    void skipSpaces();
    void skipComment();

    VarDecl* findVar(const std::string&);

    void parse(const std::string&);

    bool readIdentifier(std::string&);

    Decl* parsePrint();
    void parseVar();
    void parseVarAssign();

    Expr* parseNumber();
    Expr* parseExpr();
    Expr* parseTerm();
    Expr* parseFactor();
};

#endif