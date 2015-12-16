#ifndef INTERPRETER2_EVAL_HPP
#define INTERPRETER2_EVAL_HPP

#include "Visitor.hpp"

struct Expr;

struct Eval : public Visitor {
    Expr* expr = nullptr;

    virtual void visit(NumberExpr*) { }

    virtual void visit(CharExpr*) { }

    virtual void visit(AddExpr*);

    virtual void visit(SubExpr*);

    virtual void visit(MulExpr*);

    virtual void visit(DivExpr*);

    virtual void visit(NotExpr*);

    virtual void visit(NegateExpr*);

    virtual void visit(ArrayExpr*);

    virtual void visit(IndexExpr*);

    virtual void visit(IndexAssignExpr*);
};

#endif
