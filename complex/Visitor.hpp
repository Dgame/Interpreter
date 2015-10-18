#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <ostream>
#include <cassert>
#include <memory>

#include "types.hpp"

struct Expr;
struct NullExpr;
struct VarExpr;
struct ArrayExpr;
struct IndexExpr;
struct IntExpr;
struct FloatExpr;
struct CharExpr;
struct StringExpr;
struct NegExpr;
struct ParenExpr;
struct AddExpr;
struct SubExpr;
struct MulExpr;
struct DivExpr;
struct ModExpr;
struct BitAndExpr;
struct BitOrExpr;

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

    virtual void visit(const CharExpr*) {
        assert(0);
    }

    virtual void visit(const StringExpr*) {
        assert(0);
    }

    virtual void visit(const NegExpr*) {
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
};

struct EvalVisitor : public Visitor {
    f32_t value = 0;

    EvalVisitor() = default;
    explicit EvalVisitor(const Expr*);

    virtual void visit(const VarExpr*);
    virtual void visit(const IndexExpr*);
    virtual void visit(const IntExpr*);
    virtual void visit(const FloatExpr*);
    virtual void visit(const NegExpr*);
    virtual void visit(const ParenExpr*);
    virtual void visit(const AddExpr*);
    virtual void visit(const SubExpr*);
    virtual void visit(const MulExpr*);
    virtual void visit(const DivExpr*);
    virtual void visit(const ModExpr*);
    virtual void visit(const BitAndExpr*);
    virtual void visit(const BitOrExpr*);
};

struct PrintVisitor : public Visitor {
    std::ostream& _out;

    explicit PrintVisitor(std::ostream&);

    virtual void visit(const NullExpr*);
    virtual void visit(const VarExpr*);
    virtual void visit(const ArrayExpr*);
    virtual void visit(const IndexExpr*);
    virtual void visit(const IntExpr*);
    virtual void visit(const FloatExpr*);
    virtual void visit(const CharExpr*);
    virtual void visit(const StringExpr*);
    virtual void visit(const NegExpr*);
    virtual void visit(const ParenExpr*);
    virtual void visit(const AddExpr*);
    virtual void visit(const SubExpr*);
    virtual void visit(const MulExpr*);
    virtual void visit(const DivExpr*);
    virtual void visit(const ModExpr*);
    virtual void visit(const BitAndExpr*);
    virtual void visit(const BitOrExpr*);
};

struct OutputVisitor : public Visitor {
    std::ostream& _out;

    explicit OutputVisitor(const Expr*, std::ostream&);

    virtual void visit(const NullExpr*);
    virtual void visit(const VarExpr*);
    virtual void visit(const ArrayExpr*);
    virtual void visit(const IndexExpr*);
    virtual void visit(const IntExpr*);
    virtual void visit(const FloatExpr*);
    virtual void visit(const CharExpr*);
    virtual void visit(const StringExpr*);
    virtual void visit(const NegExpr*);
    virtual void visit(const ParenExpr*);
    virtual void visit(const AddExpr*);
    virtual void visit(const SubExpr*);
    virtual void visit(const MulExpr*);
    virtual void visit(const DivExpr*);
    virtual void visit(const ModExpr*);
    virtual void visit(const BitAndExpr*);
    virtual void visit(const BitOrExpr*);
};

struct IndexVisitor : public Visitor {
    Visitor* _visitor;
    u32_t index = 0;

    explicit IndexVisitor(const IndexExpr*, Visitor*);

    virtual void visit(const ArrayExpr*);
    virtual void visit(const StringExpr*);
};

#endif