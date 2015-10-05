#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <ostream>
#include <memory>
#include <cmath>

#include "types.hpp"
#include "error.hpp"

struct Expr {
    virtual ~Expr() { }

    virtual std::ostream& print(std::ostream&) const = 0;
    virtual f32_t eval() const = 0;
};

struct VarExpr : public Expr {
    Expr* exp;

    explicit VarExpr(Expr*);

    virtual std::ostream& print(std::ostream&) const;

    virtual f32_t eval() const {
        return this->exp->eval();
    }
};

struct IntExpr : public Expr {
    i32_t ival = 0;

    explicit IntExpr(i32_t);

    virtual std::ostream& print(std::ostream&) const;

    virtual f32_t eval() const {
        return this->ival;
    }
};

struct FloatExpr : public Expr {
    f32_t dval = 0;

    explicit FloatExpr(f32_t);

    virtual std::ostream& print(std::ostream&) const;

    virtual f32_t eval() const {
        return this->dval;
    }
};

struct UnaExpr : public Expr {
    std::unique_ptr<Expr> exp;

    explicit UnaExpr(Expr*);
};

struct NegExpr : public UnaExpr {
    explicit NegExpr(Expr*);

    virtual std::ostream& print(std::ostream&) const;

    virtual f32_t eval() const {
        return this->exp->eval() * -1;
    }
};

struct ParenExpr : public UnaExpr {
    explicit ParenExpr(Expr*);

    virtual std::ostream& print(std::ostream&) const;

    virtual f32_t eval() const {
        return this->exp->eval();
    }
};

struct BinExpr : public Expr {
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    explicit BinExpr(Expr*, Expr*);
};

struct AddExpr : public BinExpr {
    explicit AddExpr(Expr*, Expr*);

    virtual std::ostream& print(std::ostream&) const;

    virtual f32_t eval() const {
        return this->lhs->eval() + this->rhs->eval();
    }
};

struct SubExpr : public BinExpr {
    explicit SubExpr(Expr*, Expr*);

    virtual std::ostream& print(std::ostream&) const;

    virtual f32_t eval() const {
        return this->lhs->eval() - this->rhs->eval();
    }
};

struct MulExpr : public BinExpr {
    explicit MulExpr(Expr*, Expr*);

    virtual std::ostream& print(std::ostream&) const;

    virtual f32_t eval() const {
        return this->lhs->eval() * this->rhs->eval();
    }
};

struct DivExpr : public BinExpr {
    explicit DivExpr(Expr*, Expr*);

    virtual std::ostream& print(std::ostream&) const;

    virtual f32_t eval() const {
        return this->lhs->eval() / this->rhs->eval();
    }
};

struct ModExpr : public BinExpr {
    explicit ModExpr(Expr*, Expr*);

    virtual std::ostream& print(std::ostream&) const;

    virtual f32_t eval() const {
        return std::fmod(this->lhs->eval(), this->rhs->eval());
    }
};

#endif