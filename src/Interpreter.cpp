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

Interpreter::Interpreter(const std::string& filename) : _scope(new Scope) {
    _lex.load(filename);
}

void Interpreter::parse() {
    Tok type = Tok::None;

    while (true) {
        const Token tok = _lex.peek();
        type = tok.type;

        switch (type) {
            case Tok::Mutable:
            case Tok::Immutable:
                this->parseVariable(tok);
                continue;
            case Tok::Identifier:
                this->parseAssignment(tok);
                continue;
            case Tok::Output:
                this->parsePrint(tok);
                continue;
            default:
                break;
        }

        break;
    }

    ensure(type == Tok::Eof, "Invalid end of file");
}

void Interpreter::parseVariable(const Token& tok) {
    ensure(tok.type == Tok::Mutable || tok.type == Tok::Immutable, "Expected 'var' or 'let'");
    _lex.confirm();

    const Token id = _lex.peek();
    ensure(id.type == Tok::Identifier, "Expected a valid variable name @ ", id.cursor.lineNr);
    _lex.confirm();

    VarDecl* vd = _scope->findVariable(id.identifier);
    ensure(vd == nullptr, "A variable with name '", id.identifier, "' already exist");

    this->expect(Tok::Assign, __LINE__);
    Expr* exp = this->parseExpr();
    this->expect(Tok::Semicolon, __LINE__);

    vd = new VarDecl(id.identifier, exp, tok.type == Tok::Immutable);
    _scope->addVariable(vd);
}

void Interpreter::parseAssignment(const Token& tok) {
    ensure(tok.type == Tok::Identifier, "Expected valid identifier for assignment");
    _lex.confirm();

    VarDecl* vd = _scope->findVariable(tok.identifier);
    ensure(vd != nullptr, "Cannot assign unknown variable ", tok.identifier, " @ ", tok.cursor.lineNr);
    ensure(!vd->isConst(), "Cannot modify const variable ", vd->getName(), " @ ", tok.cursor.lineNr);

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
    this->expect(Tok::Semicolon, __LINE__);

    if (hasIndex) {
        if (index)
            vd->assignAt(index, exp);
        else
            vd->append(exp);
    } else
        vd->assign(exp);
}

void Interpreter::parsePrint(const Token& tok) {
    ensure(tok.type == Tok::Output, "Expected 'print'");
    _lex.confirm();

    std::unique_ptr<PrintDecl> decl = std::make_unique<PrintDecl>(std::cout);

    while (true) {
        Expr* exp = this->parseExpr();
        if (!exp) {
            error("Expected valid Expression for output");
            break;
        } else
            decl->add(exp);

        if (_lex.peek().type != Tok::Comma)
            break;
        _lex.confirm();
    }

    this->expect(Tok::Semicolon, __LINE__);
    decl->eval();
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
        default:
            break;
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

            default:
                break;
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
            default:
                error("Unexpected unary");
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