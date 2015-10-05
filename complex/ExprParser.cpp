#include "ExprParser.hpp"
#include "Expression.hpp"

#include <fstream>
#include <locale>
#include <map>

bool ExprParser::accept(char c) {
    this->skipSpaces();

    if (_loc.getCurrent() == c) {
        _loc.next();

        return true;
    }

    return false;
}

bool ExprParser::expect(char c) {
    if (!this->accept(c)) {
        error("Expected ", c);

        return false;
    }

    return true;
}

bool ExprParser::accept(const std::string& id) {
    _loc.track();

    bool abort = false;
    for (char c : id) {
        if (!this->accept(c)) {
            abort = true;
            break;
        }
    }

    if (abort) {
        _loc.backtrack();
    }

    return !abort;
}

void ExprParser::skipSpaces() {
    while (!_loc.eof() && std::isspace(_loc.getCurrent())) {
        _loc.next();
    }
}

void ExprParser::skipComment() {
    while (!_loc.eof() && this->accept('#')) {
        const u32_t line = _loc.lineNr;
        while (!_loc.eof()) {
            _loc.next();
            if (line < _loc.lineNr)
                break;
        }
    }
}

VarDecl* ExprParser::findVar(const std::string& id) {
    if (!id.empty()) {
        for (auto it = _vars.rbegin(); it != _vars.rend(); it++) {
            if ((*it)->name == id)
                return it->get();
        }

        error("Unknown variable ", id);
    }

    return nullptr;
}

void ExprParser::parse(const std::string& filename) {
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
            if (decl)
                decl->eval();
        }
    } else {
        error("Invalid file ", filename);
    }
}

bool ExprParser::readIdentifier(std::string& id) {
    this->skipSpaces();

    if (_loc.eof()) {
        error("Expected identifier, not EOF");

        return false;
    }

    if (!std::isalpha(_loc.getCurrent())) {
        error("Expected identifier, not ", _loc.getCurrent());

        return false;
    }

    _loc.track();

    id.reserve(32);
    while (!_loc.eof() && std::isalnum(_loc.getCurrent())) {
        id += _loc.getCurrent();
        _loc.next();
    }

    if (id == "print" ||
        id == "let" ||
        id == "var")
    {
        _loc.backtrack();
        return false;
    }

    return true;
}

Decl* ExprParser::parsePrint() {
    if (this->accept("print")) {
        PrintDecl* decl = new PrintDecl();
        while (!_loc.eof()) {
            Expr* exp = this->parseExpr();
            if (exp)
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

void ExprParser::parseVar() {
    const bool isVar = this->accept("var");
    const bool isLet = this->accept("let");

    if (isVar || isLet) {
        std::string name;

        if (!this->readIdentifier(name)) {
            return error("Expected valid variable name");
        }

        this->expect('=');
        Expr* exp = this->parseExpr();
        this->expect(';');

        VarDecl* decl = new VarDecl(name, exp);
        decl->isConst = isLet;

        _vars.emplace_back(decl);
    } else {
        this->parseVarAssign();
    }
}

void ExprParser::parseVarAssign() {
    std::string name;

    if (this->readIdentifier(name)) {
        VarDecl* vd = this->findVar(name);

        if (vd->isConst) {
            error("Cannot modify const variable ", name);
        } else {
            this->expect('=');
            Expr* exp = this->parseExpr();
            this->expect(';');

            VarDecl* decl = new VarDecl(vd->name, exp);

            _vars.emplace_back(decl);
        }
    }
}

Expr* ExprParser::parseNumber() {
    this->skipSpaces();

    auto isValid = [this]() -> bool {
        if (_loc.eof())
            return false;
        const char c = _loc.getCurrent();
        return std::isdigit(c);
    };

    if (!isValid())
        return nullptr;

    i32_t num = 0;
    do {
        num *= 10;
        num += _loc.getCurrent() - '0';

        _loc.next();
    } while (isValid());

    if (this->accept('.')) {
        f32_t pot = 1;
        i32_t dec = 0;
        do {
            pot *= 10;
            dec *= 10;
            dec += _loc.getCurrent() - '0';

            _loc.next();
        } while (isValid());

        const f32_t dval = num + (dec / pot);

        return new FloatExpr(dval);
    }

    return new IntExpr(num);
}

Expr* ExprParser::parseExpr() {
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

Expr* ExprParser::parseTerm() {
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

Expr* ExprParser::parseFactor() {
    const bool negate = this->accept('-');

    std::string id;

    Expr* expr = this->parseNumber();
    if (!expr) {
        if (this->accept('(')) {
            expr = this->parseExpr();
            expr = new ParenExpr(expr);
            this->expect(')');
        } else if (this->readIdentifier(id)) {
            VarDecl* vd = this->findVar(id);
            if (vd) {
                expr = new VarExpr(vd->exp.get());
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