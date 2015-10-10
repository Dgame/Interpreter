#include "Interpreter.hpp"
#include "Expression.hpp"

#include <fstream>
#include <locale>

bool Interpreter::accept(char c) {
    this->skipSpaces();

    if (_loc.getCurrent() == c) {
        _loc.next();

        return true;
    }

    return false;
}

bool Interpreter::accept(Tok tok) {
    _loc.track();

    const Token token = this->readIdentifier();
    if (token.type != tok) {
        _loc.backtrack();

        return false;
    }

    return true;
}

bool Interpreter::expect(char c) {
    if (!this->accept(c)) {
        error("Expected ", c);

        return false;
    }

    return true;
}

bool Interpreter::expect(Tok tok) {
    if (!this->accept(tok)) {
        error("Expected Tok ", static_cast<i32_t>(tok));

        return false;
    }

    return true;
}

void Interpreter::skipSpaces() {
    while (!_loc.eof() && std::isspace(_loc.getCurrent())) {
        _loc.next();
    }
}

void Interpreter::skipComment() {
    while (!_loc.eof() && this->accept('#')) {
        const u32_t line = _loc.lineNr;
        while (!_loc.eof()) {
            _loc.next();
            if (line < _loc.lineNr)
                break;
        }
    }
}

VarDecl* Interpreter::findVar(const std::string& id) {
    if (!id.empty()) {
        for (auto it = _vars.rbegin(); it != _vars.rend(); it++) {
            if ((*it)->name == id)
                return it->get();
        }

        error("Unknown variable ", id);
    }

    return nullptr;
}

Token Interpreter::readIdentifier() {
    this->skipSpaces();

    if (_loc.eof()) {
        return Token(Tok::None);
    }

    if (!std::isalpha(_loc.getCurrent())) {
        error("Expected identifier, not ", _loc.getCurrent());

        return Token(Tok::None);
    }

    _loc.track();

    std::string str;
    str.reserve(32);

    while (!_loc.eof() && std::isalnum(_loc.getCurrent())) {
        str += _loc.getCurrent();
        _loc.next();
    }

    return Token::Identify(str);
}

Token Interpreter::readNumber() {
    this->skipSpaces();

    auto isValid = [this]() -> bool {
        if (_loc.eof())
            return false;
        const char c = _loc.getCurrent();
        return std::isdigit(c);
    };

    if (!isValid())
        return Token(Tok::None);

    Token token(Tok::Integer);

    i32_t num = 0;
    do {
        num *= 10;
        num += _loc.getCurrent() - '0';

        _loc.next();
    } while (isValid());

    if (this->accept('.')) {
        token.type = Tok::Decimal;

        f32_t pot = 1;
        i32_t dec = 0;
        do {
            pot *= 10;
            dec *= 10;
            dec += _loc.getCurrent() - '0';

            _loc.next();
        } while (isValid());

        token.decimal = num + (dec / pot);
    } else {
        token.integer = num;
    }

    return token;
}

void Interpreter::parse(const std::string& filename) {
    std::ifstream stream(filename);
    if (stream.good()) {
        /*
         * Get the size of the file
         */
        stream.seekg(0, std::ios::end);
        const std::streampos len = stream.tellg();
        stream.seekg(0, std::ios::beg);
        /*
         * Read the whole file into the buffer.
         */
        std::vector<char> buffer(len);
        stream.read(&buffer[0], len);

        _loc = Loc(&buffer.front(), &buffer.back());

        /*
         * Ignore possible header
         */
        while (!_loc.eof() && !std::isalnum(_loc.getCurrent())) {
            _loc.next();
        }

        while (!_loc.eof()) {
            this->skipSpaces();
            this->skipComment();

            this->parseVar();

            std::unique_ptr<Decl> decl(this->parsePrint());
            if (decl) {
#if 0
                decl->print(std::cout) << std::endl;
#endif
                decl->eval();
            }
        }
    } else {
        error("Invalid file ", filename);
    }
}

void Interpreter::parseVar() {
    bool isVar = this->accept(Tok::Mutable);
    bool isLet = false;

    if (!isVar)
        isLet = this->accept(Tok::Immutable);

    if (isVar || isLet) {
        const Token tok = this->readIdentifier();

        if (tok.type != Tok::Identifier) {
            return error("Expected valid variable name, not ", tok.identifier);
        }

        this->expect('=');
        Expr* exp = this->parseExpr();
        if (!exp) {
            return error("Expected valid Expression as assignment");
        }
        this->expect(';');

        VarDecl* decl = new VarDecl(tok.identifier, exp);
        decl->isConst = isLet;

        _vars.emplace_back(decl);
    } else {
        this->parseVarAssign();
    }
}

void Interpreter::parseVarAssign() {
    _loc.track();

    const Token tok = this->readIdentifier();

    if (tok.type == Tok::Identifier) {
        VarDecl* vd = this->findVar(tok.identifier);
        if (!vd) {
            return error("Cannot assign unknown variable ", tok.identifier);
        }

        if (vd->isConst) {
            error("Cannot modify const variable ", vd->name);
        } else {
            this->expect('=');
            Expr* exp = this->parseExpr();
            if (!exp) {
                return error("Expected valid Expression as assignment");
            }
            this->expect(';');

            VarDecl* decl = new VarDecl(vd->name, exp);

            _vars.emplace_back(decl);
        }
    } else {
        _loc.backtrack();
    }
}

Decl* Interpreter::parsePrint() {
    if (this->accept(Tok::Output)) {
        PrintDecl* decl = new PrintDecl();
        while (!_loc.eof()) {
            Expr* exp = this->parseExpr();
            if (!exp) {
                error("Expected valid Expression for output");

                delete decl;

                return nullptr;
            }

            decl->add(exp);

            if (!this->accept(',')) {
                break;
            }
        }

        this->expect(';');

        return decl;
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

Expr* Interpreter::parseExpr() {
    Expr* lhs = this->parseTerm();
    if (!lhs)
        return nullptr;

    while (!_loc.eof()) {
        if (this->accept('+')) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after +");
                break;
            }

            lhs = new AddExpr(lhs, rhs);
        } else if (this->accept('-')) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error("Expected factor after -");
                break;
            }

            lhs = new SubExpr(lhs, rhs);
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
                error("Expected factor after *");
                break;
            }

            lhs = new MulExpr(lhs, rhs);
        } else if (this->accept('/')) {
            Expr* rhs = this->parseFactor();
            if (!rhs) {
                error("Expected factor after /");
                break;
            }

            lhs = new DivExpr(lhs, rhs);
        } else if (this->accept('%')) {
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
    const bool negate = this->accept('-');

    std::string id;

    Expr* expr = this->parseNumber();
    if (!expr) {
        if (this->accept('(')) {
            expr = this->parseExpr();
            expr = new ParenExpr(expr);
            this->expect(')');
        } else {
            const Token tok = this->readIdentifier();

            if (tok.type != Tok::Identifier) {
                error("Expected math factor or variable name");

                return nullptr;
            }

            VarDecl* vd = this->findVar(tok.identifier);
            if (vd) {
                expr = new VarExpr(vd->exp.get());
            } else {
                error("Expected variable: ", tok.identifier);

                return nullptr;
            }
        }
    }

    if (negate) {
        if (!expr)
            error("Nothing that can be negated");
        else
            return new NegExpr(expr);
    }

    return expr;
}