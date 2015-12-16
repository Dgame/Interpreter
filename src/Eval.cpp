#include "Eval.hpp"
#include "Expr.hpp"
#include "Math.hpp"

void Eval::visit(AddExpr* exp) {
    Math m;
    exp->accept(&m);

    this->expr = new NumberExpr(m.value);
}

void Eval::visit(SubExpr* exp) {
    Math m;
    exp->accept(&m);

    this->expr = new NumberExpr(m.value);
}

void Eval::visit(MulExpr* exp) {
    Math m;
    exp->accept(&m);

    this->expr = new NumberExpr(m.value);
}

void Eval::visit(DivExpr* exp) {
    Math m;
    exp->accept(&m);

    this->expr = new NumberExpr(m.value);
}

void Eval::visit(NotExpr* exp) {
    Math m;
    exp->accept(&m);

    this->expr = new NumberExpr(m.value);
}

void Eval::visit(NegateExpr* exp) {
    Math m;
    exp->accept(&m);

    this->expr = new NumberExpr(m.value);
}

void Eval::visit(ArrayExpr* exp) {
    for (auto& item : exp->expressions) {
        Eval e;
        item->accept(&e);

        if (e.expr) {
            item.reset(e.expr);
        }
    }
}

void Eval::visit(IndexExpr* exp) {
    if (!exp->index->isAtomic()) {
        Math m;
        exp->index->accept(&m);
        exp->index.reset(new NumberExpr(m.value));
    }

    const i32_t i = exp->index->asInteger();
    assert(i >= 0, "Index underflow");
    const u32_t idx = static_cast<u32_t>(i);

    this->expr = exp->array->getAt(idx)->syntaxCopy();
}

void Eval::visit(IndexAssignExpr* exp) {
    if (!exp->index->isAtomic()) {
        Math m;
        exp->index->accept(&m);
        exp->index.reset(new NumberExpr(m.value));
    }

    const i32_t i = exp->index->asInteger();
    assert(i >= 0, "Index underflow");
    const u32_t idx = static_cast<u32_t>(i);

    if (exp->assignment->rvalue)
        exp->array->setAt(idx, exp->assignment.release());
    else
        exp->array->setAt(idx, exp->assignment->syntaxCopy());
}