//
// Created by Randy on 31.10.2015.
//

#include "Index.hpp"
#include "Eval.hpp"
#include "Expression.hpp"
#include "unique.hpp"

Index::Index(const IndexExpr* ie, Visitor* v) : _visitor(v) {
    Eval ev;
    ie->index->accept(&ev);

    if (ev.value < 0)
        error("Index below 0");

    this->index = static_cast<u32_t>(ev.value);

    ie->exp->accept(this);
}

void Index::visit(const ArrayExpr* ae) {
    if (this->index >= ae->exps.size())
        error("Invalid index ", this->index, "; max ", ae->exps.size());

    ae->exps.at(this->index)->accept(_visitor);
}

void Index::visit(const StringExpr* se) {
    if (this->index >= se->value.length())
        error("Invalid index ", this->index, "; max ", se->value.length());

    auto ce = std::make_unique<CharExpr>(se->value[this->index]);
    ce->accept(_visitor);
}