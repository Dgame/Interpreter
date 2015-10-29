#ifndef DECLARATION_HPP
#define DECLARATION_HPP

#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

#include "types.hpp"

class Decl {
public:
    virtual ~Decl() { }

    virtual std::ostream& print(std::ostream&) const = 0;
    virtual void eval() const = 0;
};

struct Expr;

class VarDecl : public Decl {
private:
    bool _isConst = false;
    std::string _name;
    std::shared_ptr<Expr> _exp;

public:
    explicit VarDecl(const std::string&, Expr*, bool);

    const std::shared_ptr<Expr>& getExpr() const {
        return _exp;
    }

    const std::string& getName() const {
        return _name;
    }

    bool isConst() const {
        return _isConst;
    }

    void assign(Expr*);
    void assignAt(Expr*, Expr*);
    void append(Expr*);

    virtual std::ostream& print(std::ostream&) const;

    virtual void eval() const {
        assert(0);
    }
};

class PrintDecl : public Decl {
private:
    std::vector<std::unique_ptr<Expr>> _exps;

public:
    void add(Expr*);

    virtual std::ostream& print(std::ostream&) const;
    virtual void eval() const;
};

#endif