#ifndef EVAL_VISITOR_HPP
#define EVAL_VISITOR_HPP

#include "Visitor.hpp"
#include "types.hpp"

struct EvalVisitor : public Visitor {
    f32_t value = 0;

    EvalVisitor() = default;
    explicit EvalVisitor(const Expr*);

    virtual void visit(const VarExpr*);
    virtual void visit(const IndexExpr*);
    virtual void visit(const IntExpr*);
    virtual void visit(const FloatExpr*);
    virtual void visit(const BoolExpr*);
    virtual void visit(const NegationExpr*);
    virtual void visit(const NotExpr*);
    virtual void visit(const ParenExpr*);
    virtual void visit(const AddExpr*);
    virtual void visit(const SubExpr*);
    virtual void visit(const MulExpr*);
    virtual void visit(const DivExpr*);
    virtual void visit(const ModExpr*);
    virtual void visit(const BitAndExpr*);
    virtual void visit(const BitOrExpr*);
    virtual void visit(const BitNotExpr*);
    virtual void visit(const BitXorExpr*);
};

#endif