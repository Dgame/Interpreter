//
// Created by Randy on 31.10.2015.
//

#include "Print.hpp"
#include "Expression.hpp"

Print::Print(std::ostream& out) : _out(out) { }

void Print::visit(const NullExpr*) {
    _out << "null";
}

void Print::visit(const VarExpr* ve) {
    _out << "<var : ";
    ve->exp->accept(this);
    _out << '>';
}

void Print::visit(const ArrayExpr* ae) {
    u32_t i = 0;

    _out << '[';
    for (auto& exp : ae->exps) {
        if (i != 0)
            _out << ',';

        exp->accept(this);

        i++;
    }
    _out << ']';
}

void Print::visit(const IndexExpr* ie) {
    ie->exp->accept(this);
    _out << '[';
    ie->index->accept(this);
    _out <<  ']';
}

void Print::visit(const IntExpr* ie) {
    _out << ie->value;
}

void Print::visit(const FloatExpr* fe) {
    _out << fe->value;
}

void Print::visit(const BoolExpr* be) {
    _out << (be->value ? "true" : "false");
}

void Print::visit(const CharExpr* ce) {
    _out << ce->value;
}

void Print::visit(const StringExpr* se) {
    _out << se->value;
}

void Print::visit(const NegationExpr* ne) {
    _out << '-';
    ne->exp->accept(this);
}

void Print::visit(const NotExpr* ne) {
    _out << '!';
    ne->exp->accept(this);
}

void Print::visit(const ParenExpr* pe) {
    _out << '(';
    pe->exp->accept(this);
    _out << ')';
}

void Print::visit(const AddExpr* add) {
    add->lhs->accept(this);
    _out << " + ";
    add->rhs->accept(this);
}

void Print::visit(const SubExpr* sub) {
    sub->lhs->accept(this);
    _out << " - ";
    sub->rhs->accept(this);
}

void Print::visit(const MulExpr* mul) {
    mul->lhs->accept(this);
    _out << " * ";
    mul->rhs->accept(this);
}

void Print::visit(const DivExpr* div) {
    div->lhs->accept(this);
    _out << " / ";
    div->rhs->accept(this);
}

void Print::visit(const ModExpr* mod) {
    mod->lhs->accept(this);
    _out << " % ";
    mod->rhs->accept(this);
}

void Print::visit(const BitAndExpr* bae) {
    bae->lhs->accept(this);
    _out << " & ";
    bae->rhs->accept(this);
}

void Print::visit(const BitOrExpr* boe) {
    boe->lhs->accept(this);
    _out << " | ";
    boe->rhs->accept(this);
}

void Print::visit(const BitNotExpr* bne) {
    _out << " ~ ";
    bne->exp->accept(this);
}

void Print::visit(const BitXorExpr* bxe) {
    bxe->lhs->accept(this);
    _out << " ^ ";
    bxe->rhs->accept(this);
}