#ifndef MATH_HPP
#define MATH_HPP

#include "Visitor.hpp"
#include "types.hpp"

struct Math : public Visitor {
    f32_t value;

    virtual void visit(NumberExpr*) override;
    virtual void visit(CharExpr*) override;

    virtual void visit(NotExpr*) override;
    virtual void visit(NegateExpr*) override;

    virtual void visit(AddExpr*) override;
    virtual void visit(SubExpr*) override;
    virtual void visit(MulExpr*) override;
    virtual void visit(DivExpr*) override;

    virtual void visit(ArrayExpr*) override {
        assert(0, "ArrayExpr not implemented");
    }

    virtual void visit(IndexAssignExpr*) override {
        assert(0, "IndexAssignExpr not implemented");
    }
};

#endif