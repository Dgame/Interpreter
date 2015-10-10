#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <ostream>

#include "types.hpp"

struct Expr;
struct VarExpr;
struct ArrayExpr;
struct IntExpr;
struct FloatExpr;
struct StringExpr;
struct NegExpr;
struct ParenExpr;
struct AddExpr;
struct SubExpr;
struct MulExpr;
struct DivExpr;
struct ModExpr;

struct Visitor {
    std::ostream& _out;

    explicit Visitor(std::ostream&);
    virtual ~Visitor() { }

    virtual void visit(const VarExpr*) = 0;
    virtual void visit(const ArrayExpr*) = 0;
    virtual void visit(const IntExpr*) = 0;
    virtual void visit(const FloatExpr*) = 0;
    virtual void visit(const StringExpr*) = 0;
    virtual void visit(const NegExpr*) = 0;
    virtual void visit(const ParenExpr*) = 0;
    virtual void visit(const AddExpr*) = 0;
    virtual void visit(const SubExpr*) = 0;
    virtual void visit(const MulExpr*) = 0;
    virtual void visit(const DivExpr*) = 0;
    virtual void visit(const ModExpr*) = 0;
};

struct EvalVisitor : public Visitor {
    f32_t value = 0;
    bool evaluated = false;

    explicit EvalVisitor(const Expr*, std::ostream&);

    EvalVisitor cloneWith(const Expr*);

    virtual void visit(const VarExpr*);
    virtual void visit(const ArrayExpr*);
    virtual void visit(const IntExpr*);
    virtual void visit(const FloatExpr*);
    virtual void visit(const StringExpr*);
    virtual void visit(const NegExpr*);
    virtual void visit(const ParenExpr*);
    virtual void visit(const AddExpr*);
    virtual void visit(const SubExpr*);
    virtual void visit(const MulExpr*);
    virtual void visit(const DivExpr*);
    virtual void visit(const ModExpr*);
};

struct PrintVisitor : public Visitor {
    explicit PrintVisitor(std::ostream&);

    virtual void visit(const VarExpr*);
    virtual void visit(const ArrayExpr*);
    virtual void visit(const IntExpr*);
    virtual void visit(const FloatExpr*);
    virtual void visit(const StringExpr*);
    virtual void visit(const NegExpr*);
    virtual void visit(const ParenExpr*);
    virtual void visit(const AddExpr*);
    virtual void visit(const SubExpr*);
    virtual void visit(const MulExpr*);
    virtual void visit(const DivExpr*);
    virtual void visit(const ModExpr*);
};

#endif