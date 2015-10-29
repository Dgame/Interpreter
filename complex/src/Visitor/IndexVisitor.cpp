#include "IndexVisitor.hpp"
#include "EvalVisitor.hpp"
#include "Expression.hpp"

IndexVisitor::IndexVisitor(const IndexExpr* ie, Visitor* v) : _visitor(v) {
    EvalVisitor ev;
    ie->index->accept(&ev);

    if (ev.value < 0)
        error("Index below 0");

    this->index = static_cast<u32_t>(ev.value);

    ie->exp->accept(this);
}

void IndexVisitor::visit(const ArrayExpr* ae) {
    if (this->index >= ae->exps.size())
        error("Invalid index ", this->index, "; max ", ae->exps.size());

    ae->exps.at(this->index)->accept(_visitor);
}

void IndexVisitor::visit(const StringExpr* se) {
    if (this->index >= se->value.length())
        error("Invalid index ", this->index, "; max ", se->value.length());

    auto ce = std::make_unique<CharExpr>(se->value[this->index]);
    ce->accept(_visitor);
}