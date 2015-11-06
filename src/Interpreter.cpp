//
// Created by Randy on 31.10.2015.
//

#include "Interpreter.hpp"
#include "Expression.hpp"
#include "Token.hpp"
#include "unique.hpp"

#include <fstream>
#include <locale>

bool Interpreter::accept(Tok type) {
    const Token* tok = _lex.peek();
    if (tok->type == type) {
        _lex.read();

        return true;
    }

    return false;
}

void Interpreter::expect(Tok type, u32_t line) {
    if (!this->accept(type)) {
        const Token* tok = _lex.read();

        const std::string exp = Token::AsString(type);
        const std::string got = Token::AsString(tok->type);

        error("Expected Token ", exp, ", got ", got, " @ line ", tok->cursor.lineNr, ". Called @ line ", line);
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
        type = _lex.peek()->type;

//        debug(" > ", Token::AsString(type));

        switch (type) {
            case Tok::Mutable:
            case Tok::Immutable:
                this->parseVariable();
                continue;
            case Tok::Identifier:
                this->parseAssignment();
                continue;
            case Tok::Output:
                this->parsePrint();
                continue;
            default:
                break;
        }

        break;
    }

    ensure(type == Tok::Eof, "Invalid end of file: ", Token::AsString(type));
}

void Interpreter::parseVariable() {
    Token* tok = _lex.read();
    ensure(tok->type == Tok::Mutable || tok->type == Tok::Immutable, "Expected 'var' or 'let' @ ", tok->cursor.lineNr);
    const Tok type = tok->type;

    tok = _lex.read();
    ensure(tok->type == Tok::Identifier, "Expected a valid variable name @ ", tok->cursor.lineNr);
    const std::string id = tok->identifier;

    VarDecl* vd = _scope->findVariable(id);
    ensure(vd == nullptr, "A variable with name '", id, "' already exist");

    this->expect(Tok::Assign, __LINE__);
    Expr* exp = this->parseExpr();
    this->expect(Tok::Semicolon, __LINE__);

    vd = new VarDecl(id, exp, type == Tok::Immutable);
    _scope->addVariable(vd);
}

void Interpreter::parseAssignment() {
    const Token* tok = _lex.read();
    ensure(tok->type == Tok::Identifier, "Expected valid identifier for assignment");

    VarDecl* vd = _scope->findVariable(tok->identifier);
    ensure(vd != nullptr, "Cannot assign unknown variable ", tok->identifier, " @ ", tok->cursor.lineNr);
    ensure(!vd->isConst(), "Cannot modify const variable ", vd->getName(), " @ ", tok->cursor.lineNr);

    bool hasIndex = false;
    Expr* index = nullptr;

    if (this->accept(Tok::OpenBracket)) {
        hasIndex = true;

        if (_lex.peek()->type != Tok::CloseBracket) {
            index = this->parseExpr();
            this->expect(Tok::CloseBracket, __LINE__);
        } else
            _lex.read();
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

void Interpreter::parsePrint() {
    debug(" > parse print");

    const Token* tok = _lex.read();
    ensure(tok->type == Tok::Output, "Expected 'print'");

    std::unique_ptr<PrintDecl> decl = std::make_unique<PrintDecl>(std::cout);

    while (true) {
        Expr* exp = this->parseExpr();
        decl->add(exp);

        if (_lex.peek()->type != Tok::Comma)
            break;

        _lex.read(); // skip ','
    }

    this->expect(Tok::Semicolon, __LINE__);
    decl->eval();
}

Expr* Interpreter::parseArrayExpr() {
    if (this->accept(Tok::OpenBracket)) {
        ArrayExpr* aexp = new ArrayExpr();

        while (_lex.peek()->type != Tok::CloseBracket) {
            Expr* exp = this->parseExpr();
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
    const Token* tok = _lex.peek();

    if (tok->type == Tok::Integer) {
        _lex.read();

        return new IntExpr(tok->integer);
    }

    if (tok->type == Tok::Decimal) {
        _lex.read();

        return new FloatExpr(tok->decimal);
    }

    return nullptr;
}

Expr* Interpreter::parseIndexOfExpr(const VarDecl* vd) {
    Expr* expr = nullptr;

    if (this->accept(Tok::OpenBracket)) {
        Expr* index = this->parseExpr();
        expr = new IndexExpr(vd, index);

        this->expect(Tok::CloseBracket, __LINE__);
    }

    return expr;
}

Expr* Interpreter::parseExpr() {
    const Token* tok = _lex.peek();

    switch (tok->type) {
        case Tok::String:
            _lex.read();

            return new StringExpr(tok->identifier);
        case Tok::Character:
            _lex.read();

            return new CharExpr(tok->character);
        case Tok::OpenBracket:
        {
            Expr* exp = this->parseArrayExpr();
            if (!exp)
                error("Expected an Expression");

            return exp;
        }
        case Tok::True:
            _lex.read();

            return new BoolExpr(true);
        case Tok::False:
            _lex.read();

            return new BoolExpr(false);
        case Tok::Null:
            _lex.read();

            return new NullExpr();
        default:
            break;
    }

    return this->parseMath();
}

Expr* Interpreter::parseMath() {
    Expr* lhs = this->parseTerm();
    if (!lhs)
        error("Expected an Expression");

    while (true) {
        if (this->accept(Tok::Plus)) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after +");
            }

            lhs = new AddExpr(lhs, rhs);
        } else if (this->accept(Tok::Minus)) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after -");
            }

            lhs = new SubExpr(lhs, rhs);
        } else if (this->accept(Tok::BitAnd)) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after &");
            }

            lhs = new BitAndExpr(lhs, rhs);
        } else if (this->accept(Tok::BitOr)) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after |");
            }

            lhs = new BitOrExpr(lhs, rhs);
        } else if (this->accept(Tok::BitXor)) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after |");
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
            }

            lhs = new MulExpr(lhs, rhs);
        } else if (this->accept(Tok::Div)) {
            Expr* rhs = this->parseFactor();
            if (!rhs) {
                error("Expected factor after /");
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
        const Token* tok = _lex.peek();

        switch (tok->type) {
            case Tok::Not:
            case Tok::Minus:
            case Tok::BitNot:
                unaries.push_back(tok->type);
                _lex.read();
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
    const Token* tok = _lex.peek();

    if (tok->type != Tok::Identifier) {
        error("Expected math factor or variable name", " @ ", tok->cursor.lineNr);

        return nullptr;
    }

    _lex.read();

    const VarDecl* vd = _scope->findVariable(tok->identifier);
    if (vd) {
        if (_lex.peek()->type == Tok::OpenBracket) {
            return this->parseIndexOfExpr(vd);
        }

        return new VarExpr(vd);
    }

    error("Expected variable: ", tok->identifier, " @ ", tok->cursor.lineNr);

    return nullptr;
}