#include "Visitor.hpp"
#include "Math.hpp"
#include "Expr.hpp"

void Visitor::visit(IndexAssignExpr* exp) {
    if (!exp->index->isAtomic()) {
        Math m;
        exp->index->accept(&m);
        exp->index.reset(new NumberExpr(m.value));
    }

    const i32_t idx = exp->index->asInteger();

    if (exp->assignment->isRvalue)
        exp->array->setAt(idx, exp->assignment.release());
    else
        exp->array->setAt(idx, exp->assignment->syntaxCopy());
}