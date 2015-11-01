//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_VISITOR_HPP
#define INTERPRETER_VISITOR_HPP

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

    virtual void fail() {
        throw "Visitor Error";
    }

    virtual void visit(const NullExpr*) {
        this->fail();
    }

    virtual void visit(const VarExpr*) {
        this->fail();
    }

    virtual void visit(const ArrayExpr*) {
        this->fail();
    }

    virtual void visit(const IndexExpr*) {
        this->fail();
    }

    virtual void visit(const IntExpr*) {
        this->fail();
    }

    virtual void visit(const FloatExpr*) {
        this->fail();
    }

    virtual void visit(const BoolExpr*) {
        this->fail();
    }

    virtual void visit(const CharExpr*) {
        this->fail();
    }

    virtual void visit(const StringExpr*) {
        this->fail();
    }

    virtual void visit(const NegationExpr*) {
        this->fail();
    }

    virtual void visit(const NotExpr*) {
        this->fail();
    }

    virtual void visit(const ParenExpr*) {
        this->fail();
    }

    virtual void visit(const AddExpr*) {
        this->fail();
    }

    virtual void visit(const SubExpr*) {
        this->fail();
    }

    virtual void visit(const MulExpr*) {
        this->fail();
    }

    virtual void visit(const DivExpr*) {
        this->fail();
    }

    virtual void visit(const ModExpr*) {
        this->fail();
    }

    virtual void visit(const BitAndExpr*) {
        this->fail();
    }

    virtual void visit(const BitOrExpr*) {
        this->fail();
    }

    virtual void visit(const BitNotExpr*) {
        this->fail();
    }

    virtual void visit(const BitXorExpr*) {
        this->fail();
    }
};

#endif //INTERPRETER_VISITOR_HPP
