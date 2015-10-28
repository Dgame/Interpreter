#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>

#include "types.hpp"
#include "error.hpp"

struct Visitor;

struct Expr {
    virtual ~Expr() { }

    virtual void accept(Visitor*) const = 0;
};

struct NullExpr : public Expr {
    virtual void accept(Visitor*) const;
};

struct VarDecl;

struct VarExpr : public Expr {
    const std::shared_ptr<Expr> exp;

    explicit VarExpr(const VarDecl*);

    virtual void accept(Visitor*) const;
};

struct ArrayExpr : public Expr {
    std::vector<std::unique_ptr<Expr>> exps;

    void add(Expr*);

    virtual void accept(Visitor*) const;
};

struct IndexExpr : public VarExpr {
    std::unique_ptr<Expr> index;

    explicit IndexExpr(const VarDecl*, Expr*);

    virtual void accept(Visitor*) const;
};

struct IntExpr : public Expr {
    i32_t value = 0;

    explicit IntExpr(i32_t);

    virtual void accept(Visitor*) const;
};

struct FloatExpr : public Expr {
    f32_t value = 0;

    explicit FloatExpr(f32_t);

    virtual void accept(Visitor*) const;
};

struct BoolExpr : public Expr {
    bool value = false;

    explicit BoolExpr(bool);

    virtual void accept(Visitor*) const;
};

struct CharExpr : public Expr {
    char value = 0;

    explicit CharExpr(char);

    virtual void accept(Visitor*) const;
};

struct StringExpr : public Expr {
    std::string value;

    explicit StringExpr(const std::string&);

    virtual void accept(Visitor*) const;
};

struct UnaExpr : public Expr {
    std::unique_ptr<Expr> exp;

    explicit UnaExpr(Expr*);
};

struct NegationExpr : public UnaExpr {
    explicit NegationExpr(Expr*);

    virtual void accept(Visitor*) const;
};

struct NotExpr : public UnaExpr {
    explicit NotExpr(Expr*);

    virtual void accept(Visitor*) const;
};

struct BitNotExpr : public UnaExpr {
    explicit BitNotExpr(Expr*);

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

struct BitAndExpr : public BinExpr {
    explicit BitAndExpr(Expr*, Expr*);

    virtual void accept(Visitor*) const;
};

struct BitOrExpr : public BinExpr {
    explicit BitOrExpr(Expr*, Expr*);

    virtual void accept(Visitor*) const;
};

struct BitXorExpr : public BinExpr {
    explicit BitXorExpr(Expr*, Expr*);

    virtual void accept(Visitor*) const;
};

#endif