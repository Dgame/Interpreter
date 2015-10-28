#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <cassert>

struct Expr;
struct NullExpr;
struct VarExpr;
struct ArrayExpr;
struct IndexExpr;
struct IntExpr;
struct FloatExpr;
struct BoolExpr;
struct CharExpr;
struct StringExpr;
struct NegationExpr;
struct NotExpr;
struct ParenExpr;
struct AddExpr;
struct SubExpr;
struct MulExpr;
struct DivExpr;
struct ModExpr;
struct BitAndExpr;
struct BitOrExpr;
struct BitNotExpr;
struct BitXorExpr;

struct Visitor {
    virtual ~Visitor() { }

    virtual void visit(const NullExpr*) {
        assert(0);
    }

    virtual void visit(const VarExpr*) {
        assert(0);
    }

    virtual void visit(const ArrayExpr*) {
        assert(0);
    }

    virtual void visit(const IndexExpr*) {
        assert(0);
    }

    virtual void visit(const IntExpr*) {
        assert(0);
    }

    virtual void visit(const FloatExpr*) {
        assert(0);
    }

    virtual void visit(const BoolExpr*) {
        assert(0);
    }

    virtual void visit(const CharExpr*) {
        assert(0);
    }

    virtual void visit(const StringExpr*) {
        assert(0);
    }

    virtual void visit(const NegationExpr*) {
        assert(0);
    }

    virtual void visit(const NotExpr*) {
        assert(0);
    }

    virtual void visit(const ParenExpr*) {
        assert(0);
    }

    virtual void visit(const AddExpr*) {
        assert(0);
    }

    virtual void visit(const SubExpr*) {
        assert(0);
    }

    virtual void visit(const MulExpr*) {
        assert(0);
    }

    virtual void visit(const DivExpr*) {
        assert(0);
    }

    virtual void visit(const ModExpr*) {
        assert(0);
    }

    virtual void visit(const BitAndExpr*) {
        assert(0);
    }

    virtual void visit(const BitOrExpr*) {
        assert(0);
    }

    virtual void visit(const BitNotExpr*) {
        assert(0);
    }

    virtual void visit(const BitXorExpr*) {
        assert(0);
    }
};

#endif