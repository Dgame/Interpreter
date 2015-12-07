#ifndef EXPR_HPP
#define EXPR_HPP

#include <vector>
#include <memory>
#include "util.hpp"
#include "types.hpp"

struct Visitor;

struct Expr {
    bool isRvalue = false;

    virtual bool isAtomic() const = 0;
    virtual Expr* syntaxCopy() = 0;
    virtual void accept(Visitor*) = 0;
};

struct NumericExpr : public Expr {
    virtual NumericExpr* syntaxCopy() override = 0;
    virtual f32_t asFloat() const   = 0;
    virtual i32_t asInteger() const = 0;
};

struct NumberExpr : public NumericExpr {
    f32_t value;

    explicit NumberExpr(f32_t);

    virtual f32_t asFloat() const override {
        return this->value;
    }

    virtual i32_t asInteger() const override {
        return static_cast<i32_t>(this->value);
    }

    virtual bool isAtomic() const override {
        return true;
    }

    virtual NumericExpr* syntaxCopy() override;

    virtual void accept(Visitor*) override;
};

struct CharExpr : public NumericExpr {
    char value;

    explicit CharExpr(char);

    virtual f32_t asFloat() const override {
        return static_cast<f32_t>(this->value);
    }

    virtual i32_t asInteger() const override {
        return static_cast<i32_t>(this->value);
    }

    virtual bool isAtomic() const override {
        return true;
    }

    virtual NumericExpr* syntaxCopy() override;

    virtual void accept(Visitor*) override;
};

template <typename T>
struct BinaryExpr : public Expr {
    std::unique_ptr<T> left;
    std::unique_ptr<T> right;

    explicit BinaryExpr(T*, T*);
};

struct BinaryNumericExpr : public BinaryExpr<NumericExpr>, public NumericExpr {
    explicit BinaryNumericExpr(NumericExpr*, NumericExpr*);

    virtual f32_t asFloat() const override {
        assert(0, "Need evaluation");
    }

    virtual i32_t asInteger() const override {
        assert(0, "Need evaluation");
    }

    virtual bool isAtomic() const override {
        return false;
    }
};

struct AddExpr : public BinaryNumericExpr {
    explicit AddExpr(NumericExpr*, NumericExpr*);

    virtual NumericExpr* syntaxCopy() override;

    virtual void accept(Visitor*) override;
};

struct SubExpr : public BinaryNumericExpr {
    explicit SubExpr(NumericExpr*, NumericExpr*);

    virtual NumericExpr* syntaxCopy() override;

    virtual void accept(Visitor*) override;
};

struct MulExpr : public BinaryNumericExpr {
    explicit MulExpr(NumericExpr*, NumericExpr*);

    virtual NumericExpr* syntaxCopy() override;

    virtual void accept(Visitor*) override;
};

struct DivExpr : public BinaryNumericExpr {
    explicit DivExpr(NumericExpr*, NumericExpr*);

    virtual NumericExpr* syntaxCopy() override;

    virtual void accept(Visitor*) override;
};

template <typename T>
struct UnaryExpr : public Expr {
    std::unique_ptr<T> exp;

    explicit UnaryExpr(T*);
};

struct UnaryNumericExpr : public UnaryExpr<NumericExpr>, public NumericExpr {
    explicit UnaryNumericExpr(NumericExpr*);

    virtual f32_t asFloat() const override {
        assert(0, "Need evaluation");
    }

    virtual i32_t asInteger() const override {
        assert(0, "Need evaluation");
    }

    virtual bool isAtomic() const override {
        return false;
    }
};

struct NotExpr : public UnaryNumericExpr {
    explicit NotExpr(NumericExpr*);

    virtual NumericExpr* syntaxCopy() override;

    virtual void accept(Visitor*) override;
};

struct NegateExpr : public UnaryNumericExpr {
    explicit NegateExpr(NumericExpr*);

    virtual NumericExpr* syntaxCopy() override;

    virtual void accept(Visitor*) override;
};

struct AccessExpr : public Expr {
    virtual void setAt(i32_t, Expr*) = 0;
    virtual Expr      * getAt(i32_t) = 0;
    virtual AccessExpr* syntaxCopy() = 0;
};

struct ArrayExpr : public AccessExpr {
    std::vector<std::unique_ptr<Expr>> expressions;

    void add(Expr*);

    void setAt(i32_t, Expr*) override;

    Expr* getAt(i32_t) override;

    virtual bool isAtomic() const override {
        return false;
    }

    virtual AccessExpr* syntaxCopy() override;

    virtual void accept(Visitor*) override;
};

struct IndexAssignExpr : public Expr {
    AccessExpr* array;
    std::unique_ptr<NumericExpr> index;
    std::unique_ptr<Expr>        assignment;

    IndexAssignExpr(AccessExpr*, NumericExpr*, Expr*);

    virtual IndexAssignExpr* syntaxCopy() override;

    virtual bool isAtomic() const override {
        return false;
    }

    virtual void accept(Visitor*) override;
};

#endif