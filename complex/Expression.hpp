#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <ostream>
#include <string>
#include <memory>
#include <cmath>

#include "types.hpp"
#include "error.hpp"

struct Visitor;

struct Expr {
    virtual ~Expr() { }

    virtual bool needEvaluation() const {
        return true;
    }

    virtual void accept(Visitor*) const = 0;
};

struct VarExpr : public Expr {
    Expr* exp;

    explicit VarExpr(Expr*);

    virtual void accept(Visitor*) const;
};

struct IntExpr : public Expr {
    i32_t value = 0;

    explicit IntExpr(i32_t);

    virtual bool needEvaluation() const {
        return false;
    }

    virtual void accept(Visitor*) const;
};

struct FloatExpr : public Expr {
    f32_t value = 0;

    explicit FloatExpr(f32_t);

    virtual bool needEvaluation() const {
        return false;
    }

    virtual void accept(Visitor*) const;
};

struct StringExpr : public Expr {
    std::string value;

    explicit StringExpr(const std::string&);

    virtual bool needEvaluation() const {
        return false;
    }

    virtual void accept(Visitor*) const;
};

struct UnaExpr : public Expr {
    std::unique_ptr<Expr> exp;

    explicit UnaExpr(Expr*);
};

struct NegExpr : public UnaExpr {
    explicit NegExpr(Expr*);

    virtual void accept(Visitor*) const;
};

struct ParenExpr : public UnaExpr {
    explicit ParenExpr(Expr*);

    virtual void accept(Visitor*) const;
};

struct BinExpr : public Expr {
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    explicit BinExpr(Expr*, Expr*);
};

struct AddExpr : public BinExpr {
    explicit AddExpr(Expr*, Expr*);

    virtual void accept(Visitor*) const;
};

struct SubExpr : public BinExpr {
    explicit SubExpr(Expr*, Expr*);

    virtual void accept(Visitor*) const;
};

struct MulExpr : public BinExpr {
    explicit MulExpr(Expr*, Expr*);

    virtual void accept(Visitor*) const;
};

struct DivExpr : public BinExpr {
    explicit DivExpr(Expr*, Expr*);

    virtual void accept(Visitor*) const;
};

struct ModExpr : public BinExpr {
    explicit ModExpr(Expr*, Expr*);

    virtual void accept(Visitor*) const;
};

#endif