//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_EVAL_HPP
#define INTERPRETER_EVAL_HPP

#include "Visitor.hpp"
#include "types.hpp"

struct Eval : public Visitor {
    f32_t value = 0;

    Eval() = default;
    explicit Eval(const Expr*);

    virtual void visit(const VarExpr*);
    virtual void visit(const IndexExpr*);
    virtual void visit(const IntExpr*);
    virtual void visit(const FloatExpr*);
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
    virtual void visit(const AndExpr*);
    virtual void visit(const OrExpr*);
    virtual void visit(const CompareExpr*);
};

#endif //INTERPRETER_EVAL_HPP
