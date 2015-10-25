#include "Interpreter.hpp"
#include "Expression.hpp"

#include <fstream>
#include <locale>

bool Interpreter::accept(Tok type, Token* the_tok) {
    const Token tok = _loc.read();
    if (the_tok)
        *the_tok = tok;
    return tok.type == type;
}

bool Interpreter::expect(Tok type) {
    Token tok;
    if (!this->accept(type, &tok)) {
        error("Expected token ", static_cast<i32_t>(type), " @ ", tok.cursor.lineNr);
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

void Interpreter::parse(const std::string& filename) : _lex(filename), _scope(new Scope()) {
    while (true) {
        this->parseVar();
        this->parsePrint();
    }
}

void Interpreter::parseVar() {
    const Token tok1 = _lex.read();

    if (tok1.type == Tok::Mutable || tok1.type == Tok::Immutable) {
        const Token tok2 = _lex.read();

        if (tok2.type != Tok::Identifier) {
            return error("Expected valid variable name, not ", tok2.identifier, " @ ", tok2.cursor.lineNr);
        }

        this->expect('=');

        Expr* exp = this->parseExpr();
        if (!exp) {
            return error("Expected valid Expression as assignment", " @ ", tok2.cursor.lineNr);
        }
        this->expect(';');

        VarDecl* vd = new VarDecl(tok.identifier, exp, tok1 == Tok::Immutable);

        _scope->addVariable(vd);
    } else {
        this->parseVarAssign();
    }
}

void Interpreter::parseVarAssign() {
    const Token tok = _lex.peek();

    if (tok.type == Tok::Identifier) {
        _lex.read();

        VarDecl* vd = _scope->findVariable(tok.identifier);
        if (!vd) {
            return error("Cannot assign unknown variable ", tok.identifier, " @ ", _loc.cursor.lineNr);
        }

        if (vd->isConst()) {
            error("Cannot modify const variable ", vd->getName(), " @ ", _loc.cursor.lineNr);
        } else {
/*
            Expr* index = nullptr;
            if (this->accept('[')) {
                index = this->parseExpr();
                this->expect(']');
            }
*/
            this->expect('=');

            Expr* exp = this->parseExpr();
            if (!exp) {
                return error("Expected valid Expression as assignment", " @ ", _loc.cursor.lineNr);
            }
            this->expect(';');
/*
            if (index) {
                exp = new IndexAssignExpr(vd->exp.release(), index, exp);
            }
*/
            vd->assign(exp);
        }
    }
}

Decl* Interpreter::parsePrint() {
    if (_lex.read().type == Tok::Output) {
        std::unique_ptr<PrintDecl> decl = std::make_unique<PrintDecl>();

        while (true) {
            Expr* exp = this->parseExpr();
            if (!exp) {
                error("Expected valid Expression for output", " @ ", _loc.cursor.lineNr);

                return nullptr;
            }

            decl->add(exp);

            if (_lex.read().type != Tok::Comma) {
                break;
            }
        }

        this->expect(';');

        return decl.release();
    }

    return nullptr;
}

Expr* Interpreter::parseString() {
    if (this->accept('"')) {
        std::string str;
        str.reserve(32);

        while (!_loc.eof() && _loc.getCurrent() != '"') {
            str += _loc.getCurrent();
            _loc.next();
        }

        this->expect('"');

        return new StringExpr(str);
    }

    return nullptr;
}

Expr* Interpreter::parseArray() {
    if (this->accept('[')) {
        ArrayExpr* aexp = new ArrayExpr();

        while (!_loc.eof()) {
            Expr* exp = this->parseExpr();
            if (!exp) {
                error("Expected valid Expression", " @ ", _loc.lineNr);
                break;
            }
            aexp->add(exp);

            if (!this->accept(',')) {
                break;
            }
        }

        this->expect(']');

        return aexp;
    }

    return nullptr;
}

Expr* Interpreter::parseNumber() {
    const Token tok = this->readNumber();

    if (tok.type == Tok::None) {
        return nullptr;
    }

    if (tok.type == Tok::Decimal) {
        return new FloatExpr(tok.decimal);
    }

    return new IntExpr(tok.integer);
}

Expr* Interpreter::parseIndexOf(const VarDecl* vd) {
    Expr* expr = nullptr;

    if (this->accept('[')) {
        Expr* index = this->parseExpr();
        if (!index) {
            error("Expected valid index Expression", " @ ", _loc.lineNr);
        } else {
            expr = new IndexExpr(vd->getExpr(), index);
        }

        this->expect(']');
    }

    return expr;
}

Expr* Interpreter::parseExpr() {
    this->skipSpaces();

    if (_loc.getCurrent() == '"')
        return this->parseString();

    if (_loc.getCurrent() == '[')
        return this->parseArray();

    Expr* lhs = this->parseTerm();
    if (!lhs)
        return nullptr;

    while (!_loc.eof()) {
        if (this->accept('+')) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after +", " @ ", _loc.lineNr);
                break;
            }

            lhs = new AddExpr(lhs, rhs);
        } else if (this->accept('-')) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after -", " @ ", _loc.lineNr);
                break;
            }

            lhs = new SubExpr(lhs, rhs);
        } else if (this->accept('&')) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after &", " @ ", _loc.lineNr);
                break;
            }

            lhs = new BitAndExpr(lhs, rhs);
        } else if (this->accept('|')) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after |", " @ ", _loc.lineNr);
                break;
            }

            lhs = new BitOrExpr(lhs, rhs);
        } else
            break;
    }

    return lhs;
}

Expr* Interpreter::parseTerm() {
    Expr* lhs = this->parseFactor();
    if (!lhs)
        return nullptr;

    while (!_loc.eof()) {
        if (this->accept('*')) {
            Expr* rhs = this->parseFactor();
            if (!rhs) {
                error("Expected factor after *", " @ ", _loc.lineNr);
                break;
            }

            lhs = new MulExpr(lhs, rhs);
        } else if (this->accept('/')) {
            Expr* rhs = this->parseFactor();
            if (!rhs) {
                error("Expected factor after /", " @ ", _loc.lineNr);
                break;
            }

            lhs = new DivExpr(lhs, rhs);
        } else if (this->accept('%')) {
            Expr* rhs = this->parseFactor();
            if (!rhs) {
                error("Expected factor after %", " @ ", _loc.lineNr);
                break;
            }

            lhs = new ModExpr(lhs, rhs);
        } else
            break;
    }

    return lhs;
}

Expr* Interpreter::parseFactor() {
    const bool negate = this->accept('-');

    std::string id;

    Expr* expr = this->parseNumber();
    if (!expr) {
        if (this->accept('(')) {
            expr = this->parseExpr();
            expr = new ParenExpr(expr);

            this->expect(')');
        } else {
            expr = this->parseVariableFactor();
        }
    }

    if (negate) {
        if (!expr)
            error("Nothing that can be negated", " @ ", _loc.lineNr);
        else
            return new NegExpr(expr);
    }

    return expr;
}

Expr* Interpreter::parseVariableFactor() {
    const Token tok = this->readIdentifier();

    if (tok.type != Tok::Identifier) {
        error("Expected math factor or variable name", " @ ", _loc.lineNr);

        return nullptr;
    }

    const VarDecl* vd = _scope->findVariable(tok.identifier);
    if (vd) {
        if (_loc.getCurrent() == '[') {
            return this->parseIndexOf(vd);
        }

        return new VarExpr(vd->getExpr());
    }

    error("Expected variable: ", tok.identifier, " @ ", _loc.lineNr);

    return nullptr;
}