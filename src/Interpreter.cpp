//
// Created by Randy on 31.10.2015.
//

#include "Interpreter.hpp"
#include "Expression.hpp"
#include "unique.hpp"

#include <fstream>
#include <locale>

bool Interpreter::accept(Tok type) {
    const Token tok = _lex.peek();
    if (tok.type == type) {
        _lex.confirm();

        return true;
    }

    return false;
}

void Interpreter::expect(Tok type, u32_t line) {
    if (!this->accept(type)) {
        const Token tok = _lex.reread();

        error("Expected Token ",
              Token::AsString(type),
              ", got ",
              Token::AsString(tok.type),
              " @ line ",
              tok.cursor.lineNr,
              ". Called @ line ",
              line
            );
    }
}

void Interpreter::pushScope() {
    Scope* sc = _scope.release();
    _scope.reset(new Scope(sc));
}

void Interpreter::popScope() {
    Scope* sc = _scope->restore();
    _scope.reset(sc);
}

void Interpreter::parse(const std::string& filename) {
    _lex.load(filename);
    _scope = std::make_unique<Scope>();

    while (true) {
        u32_t flag = 0;

        flag |= this->parseVar();
        flag |= this->parsePrint();

        // debug(flag);

        if (flag == 0)
            break;
    }
}

bool Interpreter::parseVar() {
    const Token tok1 = _lex.peek();

    if (tok1.type == Tok::Mutable || tok1.type == Tok::Immutable) {
        _lex.confirm();

        const Token tok2 = _lex.read();

        if (tok2.type != Tok::Identifier) {
            error("Expected valid variable name, not '", tok2.identifier, "' @ ", tok2.cursor.lineNr);

            return false;
        }

        const VarDecl* vde = _scope->findVariable(tok2.identifier);
        if (vde) {
            error("A variable with name '", vde->getName(), "' already exists");

            return false;
        }

        this->expect(Tok::Assign, __LINE__);

        Expr* exp = this->parseExpr();
        if (!exp) {
            error("Expected valid Expression as assignment", " @ ", tok2.cursor.lineNr);

            return false;
        }

        this->expect(Tok::Semicolon, __LINE__);

        VarDecl* vd = new VarDecl(tok2.identifier, exp, tok1.type == Tok::Immutable);
        _scope->addVariable(vd);

        return true;
    }

    return this->parseVarAssign();
}

bool Interpreter::parseVarAssign() {
    const Token tok = _lex.peek();

    if (tok.type == Tok::Identifier) {
        _lex.confirm();

        VarDecl* vd = _scope->findVariable(tok.identifier);
        if (!vd) {
            error("Cannot assign unknown variable ", tok.identifier, " @ ", tok.cursor.lineNr);

            return false;
        }

        if (vd->isConst()) {
            error("Cannot modify const variable ", vd->getName(), " @ ", tok.cursor.lineNr);

            return false;
        }

        bool hasIndex = false;
        Expr* index = nullptr;

        if (this->accept(Tok::OpenBracket)) {
            hasIndex = true;

            if (_lex.peek().type != Tok::CloseBracket) {
                index = this->parseExpr();
                this->expect(Tok::CloseBracket, __LINE__);
            } else
                _lex.confirm();
        }

        this->expect(Tok::Assign, __LINE__);

        Expr* exp = this->parseExpr();
        if (!exp) {
            error("Expected valid Expression as assignment", " @ ", tok.cursor.lineNr);

            return false;
        }

        this->expect(Tok::Semicolon, __LINE__);

        if (hasIndex) {
            if (index)
                vd->assignAt(index, exp);
            else
                vd->append(exp);
        } else
            vd->assign(exp);

        return true;
    }

    return false;
}

bool Interpreter::parsePrint() {
    if (_lex.peek().type == Tok::Output) {
        _lex.confirm();

        std::unique_ptr<PrintDecl> decl = std::make_unique<PrintDecl>();

        while (true) {
            Expr* exp = this->parseExpr();
            if (!exp) {
                error("Expected valid Expression for output");
                break;
            } else {
                decl->add(exp);
            }

            if (_lex.peek().type != Tok::Comma) {
                break;
            }

            _lex.confirm();
        }

        this->expect(Tok::Semicolon, __LINE__);

        decl->eval();

        return true;
    }

    return false;
}

Expr* Interpreter::parseArrayExpr() {
    if (this->accept(Tok::OpenBracket)) {
        ArrayExpr* aexp = new ArrayExpr();

        while (_lex.peek().type != Tok::CloseBracket) {
            Expr* exp = this->parseExpr();
            if (!exp) {
                error("Expected valid Expression");
                break;
            }
            aexp->add(exp);

            if (!this->accept(Tok::Comma)) {
                break;
            }
        }

        this->expect(Tok::CloseBracket, __LINE__);

        return aexp;
    }

    return nullptr;
}

Expr* Interpreter::parseNumericExpr() {
    const Token tok = _lex.peek();

    if (tok.type == Tok::Integer) {
        _lex.confirm();

        return new IntExpr(tok.integer);
    }

    if (tok.type == Tok::Decimal) {
        _lex.confirm();

        return new FloatExpr(tok.decimal);
    }

    return nullptr;
}

Expr* Interpreter::parseIndexOfExpr(const VarDecl* vd) {
    Expr* expr = nullptr;

    if (this->accept(Tok::OpenBracket)) {
        Expr* index = this->parseExpr();
        if (!index) {
            error("Expected valid index Expression");
        } else {
            expr = new IndexExpr(vd, index);
        }

        this->expect(Tok::CloseBracket, __LINE__);
    }

    return expr;
}

Expr* Interpreter::parseExpr() {
    const Token tok = _lex.peek();

    switch (tok.type) {
        case Tok::String:
            _lex.confirm();
            return new StringExpr(tok.identifier);
        case Tok::OpenBracket:
            return this->parseArrayExpr();
        default: break;
    }

    Expr* lhs = this->parseTerm();
    if (!lhs)
        return nullptr;

    while (true) {
        if (this->accept(Tok::Plus)) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after +");
                break;
            }

            lhs = new AddExpr(lhs, rhs);
        } else if (this->accept(Tok::Minus)) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after -");
                break;
            }

            lhs = new SubExpr(lhs, rhs);
        } else if (this->accept(Tok::BitAnd)) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after &");
                break;
            }

            lhs = new BitAndExpr(lhs, rhs);
        } else if (this->accept(Tok::BitOr)) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after |");
                break;
            }

            lhs = new BitOrExpr(lhs, rhs);
        } else if (this->accept(Tok::BitXor)) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after |");
                break;
            }

            lhs = new BitXorExpr(lhs, rhs);
        } else
            break;
    }

    return lhs;
}

Expr* Interpreter::parseTerm() {
    Expr* lhs = this->parseFactor();
    if (!lhs)
        return nullptr;

    while (true) {
        if (this->accept(Tok::Mul)) {
            Expr* rhs = this->parseFactor();
            if (!rhs) {
                error("Expected factor after *");
                break;
            }

            lhs = new MulExpr(lhs, rhs);
        } else if (this->accept(Tok::Div)) {
            Expr* rhs = this->parseFactor();
            if (!rhs) {
                error("Expected factor after /");
                break;
            }

            lhs = new DivExpr(lhs, rhs);
        } else if (this->accept(Tok::Mod)) {
            Expr* rhs = this->parseFactor();
            if (!rhs) {
                error("Expected factor after %");
                break;
            }

            lhs = new ModExpr(lhs, rhs);
        } else
            break;
    }

    return lhs;
}

Expr* Interpreter::parseFactor() {
    std::vector<Tok> unaries;
    while (true) {
        Token tok = _lex.peek();

        switch (tok.type) {
            case Tok::Not:
            case Tok::Minus:
            case Tok::BitNot:
                unaries.push_back(tok.type);
                _lex.confirm();
            continue;

            default: break;
        }

        break;
    }

    Expr* expr = this->parseNumericExpr();
    if (!expr) {
        if (this->accept(Tok::OpenParen)) {
            expr = this->parseExpr();
            expr = new ParenExpr(expr);

            this->expect(Tok::CloseParen, __LINE__);
        } else {
            expr = this->parseVariableFactor();
        }
    }

    if (!expr) {
        error("Expected a factor Expression");

        return nullptr;
    }

    for (const Tok tok : unaries) {
        switch (tok) {
            case Tok::Not:
                expr = new NotExpr(expr);
                break;
            case Tok::Minus:
                expr = new NegationExpr(expr);
                break;
            case Tok::BitNot:
                expr = new BitNotExpr(expr);
                break;
            default: error("Unexpected unary");
        }
    }

    return expr;
}

Expr* Interpreter::parseVariableFactor() {
    const Token tok = _lex.peek();

    if (tok.type != Tok::Identifier) {
        error("Expected math factor or variable name", " @ ", tok.cursor.lineNr);

        return nullptr;
    }

    _lex.confirm();

    const VarDecl* vd = _scope->findVariable(tok.identifier);
    if (vd) {
        if (_lex.peek().type == Tok::OpenBracket) {
            return this->parseIndexOfExpr(vd);
        }

        return new VarExpr(vd);
    }

    error("Expected variable: ", tok.identifier, " @ ", tok.cursor.lineNr);

    return nullptr;
}