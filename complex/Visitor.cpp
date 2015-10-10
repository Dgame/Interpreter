#include "Visitor.hpp"
#include "Expression.hpp"

#include <cassert>
#include <cmath>

Visitor::Visitor(std::ostream& out) : _out(out) { }

EvalVisitor::EvalVisitor(const Expr* exp, std::ostream& out) : Visitor(out) {
    if (exp->needEvaluation()) {
        exp->accept(this);

        if (!this->evaluated) {
            out << this->value;

            this->evaluated = true;
        }
    } else {
        exp->print(out);
    }
}

void EvalVisitor::visit(const VarExpr* ve) {
    ve->exp->accept(this);
}

void EvalVisitor::visit(const ArrayExpr* ae) {
    this->evaluated = true;

    u32_t i = 0;
    for (auto& exp : ae->exps) {
        if (i != 0)
            _out << ", ";

        EvalVisitor ev(exp.get(), _out);

        i++;
    }
}

void EvalVisitor::visit(const IntExpr* ie) {
    this->value = ie->value;
}

void EvalVisitor::visit(const FloatExpr* fe) {
    this->value = fe->value;
}

void EvalVisitor::visit(const StringExpr*) {
    assert(0);
}

void EvalVisitor::visit(const NegExpr* ne) {
    ne->exp->accept(this);
    this->value = this->value * -1;
}

void EvalVisitor::visit(const ParenExpr* pe) {
    pe->exp->accept(this);
}

void EvalVisitor::visit(const AddExpr* add) {
    add->lhs->accept(this);
    const f32_t lhs = this->value;

    add->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = lhs + rhs;
}

void EvalVisitor::visit(const SubExpr* sub) {
    sub->lhs->accept(this);
    const f32_t lhs = this->value;

    sub->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = lhs - rhs;
}

void EvalVisitor::visit(const MulExpr* mul) {
    mul->lhs->accept(this);
    const f32_t lhs = this->value;

    mul->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = lhs * rhs;
}

void EvalVisitor::visit(const DivExpr* div) {
    div->lhs->accept(this);
    const f32_t lhs = this->value;

    div->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = lhs * rhs;
}

void EvalVisitor::visit(const ModExpr* mod) {
    mod->lhs->accept(this);
    const f32_t lhs = this->value;

    mod->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = std::fmod(lhs, rhs);
}

PrintVisitor::PrintVisitor(std::ostream& out) : Visitor(out) { }

void PrintVisitor::visit(const VarExpr* ve) {
    _out << "<var : ";
    ve->exp->accept(this);
    _out << '>';
}

void PrintVisitor::visit(const ArrayExpr* ae) {
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

void PrintVisitor::visit(const IntExpr* ie) {
    _out << ie->value;
}

void PrintVisitor::visit(const FloatExpr* fe) {
    _out << fe->value;
}

void PrintVisitor::visit(const StringExpr* se) {
    _out << se->value;
}

void PrintVisitor::visit(const NegExpr* ne) {
    _out << '-';
    ne->exp->accept(this);
}

void PrintVisitor::visit(const ParenExpr* pe) {
    _out << '(';
    pe->exp->accept(this);
    _out << ')';
}

void PrintVisitor::visit(const AddExpr* add) {
    add->lhs->accept(this);
    _out << " + ";
    add->rhs->accept(this);
}

void PrintVisitor::visit(const SubExpr* sub) {
    sub->lhs->accept(this);
    _out << " - ";
    sub->rhs->accept(this);
}

void PrintVisitor::visit(const MulExpr* mul) {
    mul->lhs->accept(this);
    _out << " * ";
    mul->rhs->accept(this);
}

void PrintVisitor::visit(const DivExpr* div) {
    div->lhs->accept(this);
    _out << " / ";
    div->rhs->accept(this);
}

void PrintVisitor::visit(const ModExpr* mod) {
    mod->lhs->accept(this);
    _out << " % ";
    mod->rhs->accept(this);
}