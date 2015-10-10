#ifndef DECLARATION_HPP
#define DECLARATION_HPP

#include <ostream>
#include <string>
#include <vector>
#include <memory>

#include "types.hpp"

struct Decl {
    virtual ~Decl() { }

    virtual std::ostream& print(std::ostream&) const = 0;
    virtual void eval() const = 0;
};

struct Expr;

struct VarDecl : public Decl {
    std::string name;
    bool isConst = false;
    std::unique_ptr<Expr> exp;

    explicit VarDecl(const std::string&, Expr*);

    virtual std::ostream& print(std::ostream&) const;
    virtual void eval() const;
};

struct PrintDecl : public Decl {
    std::vector<std::unique_ptr<Expr>> exps;

    void add(Expr*);

    virtual std::ostream& print(std::ostream&) const;
    virtual void eval() const;
};

#endif