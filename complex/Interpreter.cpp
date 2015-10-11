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
        error(_loc, "Expected ", c);

        return false;
    }

    return true;
}

bool Interpreter::expect(Tok tok) {
    if (!this->accept(tok)) {
        error(_loc, "Expected Tok ", static_cast<i32_t>(tok));

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
        for (auto it = _vars.begin(); it != _vars.end(); it++) {
            if ((*it)->name == id)
                return it->get();
        }

        error(_loc, "Unknown variable ", id);
    }

    return nullptr;
}

Token Interpreter::readIdentifier() {
    this->skipSpaces();

    if (_loc.eof()) {
        return Token(Tok::None);
    }

    if (!std::isalpha(_loc.getCurrent())) {
        error(_loc, "Expected identifier, not ", _loc.getCurrent());

        return Token(Tok::None);
    }

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

        _loc = Location(&buffer.front(), &buffer.back());

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
#else
                decl->eval();
#endif
            }
        }
    } else {
        error(_loc, "Invalid file ", filename);
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
            return error(_loc, "Expected valid variable name, not ", tok.identifier);
        }

        this->expect('=');
        Expr* exp = this->parseExpr();
        if (!exp) {
            return error(_loc, "Expected valid Expression as assignment");
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
            return error(_loc, "Cannot assign unknown variable ", tok.identifier);
        }

        if (vd->isConst) {
            error(_loc, "Cannot modify const variable ", vd->name);
        } else {
            this->expect('=');
            Expr* exp = this->parseExpr();
            if (!exp) {
                return error(_loc, "Expected valid Expression as assignment");
            }
            this->expect(';');

            vd->assign(exp);
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
                error(_loc, "Expected valid Expression for output");

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
                error(_loc, "Expected valid Expression");
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
            error(_loc, "Expected valid index Expression");
        } else {
            expr = new IndexExpr(vd->exp.get(), index);
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
                error(_loc, "Expected factor after +");
                break;
            }

            lhs = new AddExpr(lhs, rhs);
        } else if (this->accept('-')) {
            Expr* rhs = this->parseTerm();
            if (!rhs) {
                error(_loc, "Expected factor after -");
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
                error(_loc, "Expected factor after *");
                break;
            }

            lhs = new MulExpr(lhs, rhs);
        } else if (this->accept('/')) {
            Expr* rhs = this->parseFactor();
            if (!rhs) {
                error(_loc, "Expected factor after /");
                break;
            }

            lhs = new DivExpr(lhs, rhs);
        } else if (this->accept('%')) {
            Expr* rhs = this->parseFactor();
            if (!rhs) {
                error(_loc, "Expected factor after %");
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
                error(_loc, "Expected math factor or variable name");

                return nullptr;
            }

            VarDecl* vd = this->findVar(tok.identifier);
            if (vd) {
                if (_loc.getCurrent() == '[') {
                    expr = this->parseIndexOf(vd);
                } else {
                    expr = new VarExpr(vd->exp.get());
                }
            } else {
                error(_loc, "Expected variable: ", tok.identifier);

                return nullptr;
            }
        }
    }

    if (negate) {
        if (!expr)
            error(_loc, "Nothing that can be negated");
        else
            return new NegExpr(expr);
    }

    return expr;
}