#include "Visitor.hpp"
#include "Expression.hpp"
#include "error.hpp"

#include <cmath>
#include <limits>

inline bool AreSame(f32_t lhs, f32_t rhs) {
    return std::fabs(lhs - rhs) < std::numeric_limits<f32_t>::epsilon();
}

/// Eval
EvalVisitor::EvalVisitor(const Expr* exp) {
    exp->accept(this);
}

void EvalVisitor::visit(const VarExpr* ve) {
    ve->exp->accept(this);
}

void EvalVisitor::visit(const IndexExpr* ie) {
    IndexVisitor(ie, this);
}

void EvalVisitor::visit(const IntExpr* ie) {
    this->value = ie->value;
}

void EvalVisitor::visit(const FloatExpr* fe) {
    this->value = fe->value;
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

void EvalVisitor::visit(const BitAndExpr* bae) {
    bae->lhs->accept(this);
    const f32_t my_lhs = this->value;

    bae->rhs->accept(this);
    const f32_t my_rhs = this->value;

    const i32_t lhs = static_cast<i32_t>(my_lhs);
    const i32_t rhs = static_cast<i32_t>(my_rhs);

    if (!AreSame(my_lhs, lhs) || !AreSame(my_rhs, rhs)) {
        error("Cannot use operator & for floats");
    }

    this->value = lhs & rhs;
}

void EvalVisitor::visit(const BitOrExpr* boe) {
    boe->lhs->accept(this);
    const f32_t my_lhs = this->value;

    boe->rhs->accept(this);
    const f32_t my_rhs = this->value;

    const i32_t lhs = static_cast<i32_t>(my_lhs);
    const i32_t rhs = static_cast<i32_t>(my_rhs);

    if (!AreSame(my_lhs, lhs) || !AreSame(my_rhs, rhs)) {
        error("Cannot use operator | for floats");
    }

    this->value = lhs | rhs;
}

/// Print
PrintVisitor::PrintVisitor(std::ostream& out) : _out(out) { }

void PrintVisitor::visit(const NullExpr*) {
    _out << "null";
}

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

void PrintVisitor::visit(const IndexExpr* ie) {
    ie->exp->accept(this);
    _out << '[';
    ie->index->accept(this);
    _out <<  ']';
}

void PrintVisitor::visit(const IntExpr* ie) {
    _out << ie->value;
}

void PrintVisitor::visit(const FloatExpr* fe) {
    _out << fe->value;
}

void PrintVisitor::visit(const CharExpr* ce) {
    _out << ce->value;
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

void PrintVisitor::visit(const BitAndExpr* bae) {
    bae->lhs->accept(this);
    _out << " & ";
    bae->rhs->accept(this);
}

void PrintVisitor::visit(const BitOrExpr* boe) {
    boe->lhs->accept(this);
    _out << " | ";
    boe->rhs->accept(this);
}

/// Output
OutputVisitor::OutputVisitor(const Expr* exp, std::ostream& out) : _out(out) {
    exp->accept(this);
}

void OutputVisitor::visit(const NullExpr*) {
    _out << "null";
}

void OutputVisitor::visit(const VarExpr* ve) {
    ve->exp->accept(this);
}

void OutputVisitor::visit(const ArrayExpr* ae) {
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

void OutputVisitor::visit(const IndexExpr* ie) {
    IndexVisitor(ie, this);
}

void OutputVisitor::visit(const IntExpr* ie) {
    _out << ie->value;
}

void OutputVisitor::visit(const FloatExpr* fe) {
    _out << fe->value;
}

void OutputVisitor::visit(const CharExpr* ce) {
    _out << ce->value;
}

void OutputVisitor::visit(const StringExpr* se) {
    _out << se->value;
}

void OutputVisitor::visit(const NegExpr* ne) {
    EvalVisitor ev;
    ne->exp->accept(&ev);

    _out << ev.value * -1;
}

void OutputVisitor::visit(const ParenExpr* pe) {
    pe->exp->accept(this);
}

void OutputVisitor::visit(const AddExpr* add) {
    EvalVisitor ev(add);

    _out << ev.value;
}

void OutputVisitor::visit(const SubExpr* sub) {
    EvalVisitor ev(sub);

    _out << ev.value;
}

void OutputVisitor::visit(const MulExpr* mul) {
    EvalVisitor ev(mul);

    _out << ev.value;
}

void OutputVisitor::visit(const DivExpr* div) {
    EvalVisitor ev(div);

    _out << ev.value;
}

void OutputVisitor::visit(const ModExpr* mod) {
    EvalVisitor ev(mod);

    _out << ev.value;
}

void OutputVisitor::visit(const BitAndExpr* bae) {
    EvalVisitor ev(bae);

    _out << ev.value;
}

void OutputVisitor::visit(const BitOrExpr* boe) {
    EvalVisitor ev(boe);

    _out << ev.value;
}


/// Index
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
