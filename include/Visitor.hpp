#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "util.hpp"

struct NumberExpr;
struct CharExpr;
struct AddExpr;
struct SubExpr;
struct MulExpr;
struct DivExpr;
struct NotExpr;
struct NegateExpr;
struct ArrayExpr;
struct IndexExpr;
struct IndexAssignExpr;

struct Visitor {
    virtual void visit(NumberExpr*) {
        assert(0, "NumberExpr not implemented");
    }

    virtual void visit(CharExpr*) {
        assert(0, "CharExpr not implemented");
    }

    virtual void visit(AddExpr*) {
        assert(0, "AddExpr not implemented");
    }

    virtual void visit(SubExpr*) {
        assert(0, "SubExpr not implemented");
    }

    virtual void visit(MulExpr*) {
        assert(0, "MulExpr not implemented");
    }

    virtual void visit(DivExpr*) {
        assert(0, "DivExpr not implemented");
    }

    virtual void visit(NotExpr*) {
        assert(0, "NotExpr not implemented");
    }

    virtual void visit(NegateExpr*) {
        assert(0, "NegateExpr not implemented");
    }

    virtual void visit(ArrayExpr*) {
        assert(0, "ArrayExpr not implemented");
    }

    virtual void visit(IndexExpr*) {
        assert(0, "IndexExpr not implemented");
    }

    virtual void visit(IndexAssignExpr*) {
        assert(0, "IndexAssignExpr not implemented");
    }
};

#endif