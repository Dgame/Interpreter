#include "EvalVisitor.hpp"
#include "IndexVisitor.hpp"
#include "Expression.hpp"

#include <limits>

inline bool AreSame(f32_t lhs, f32_t rhs) {
    return std::fabs(lhs - rhs) < std::numeric_limits<f32_t>::epsilon();
}

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

void EvalVisitor::visit(const BoolExpr* be) {
    this->value = static_cast<i32_t>(be->value);
}

void EvalVisitor::visit(const NegationExpr* ne) {
    ne->exp->accept(this);
    this->value = this->value * -1;
}

void EvalVisitor::visit(const NotExpr* ne) {
    ne->exp->accept(this);
    this->value = !static_cast<i32_t>(this->value);
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

void EvalVisitor::visit(const BitNotExpr* bne) {
    bne->exp->accept(this);
    const f32_t my_lhs = this->value;

    const i32_t lhs = static_cast<i32_t>(my_lhs);

    if (!AreSame(my_lhs, lhs)) {
        error("Cannot use operator ~ for float");
    }

    this->value = ~lhs;
}

void EvalVisitor::visit(const BitXorExpr* bxe) {
    bxe->lhs->accept(this);
    const f32_t my_lhs = this->value;

    bxe->rhs->accept(this);
    const f32_t my_rhs = this->value;

    const i32_t lhs = static_cast<i32_t>(my_lhs);
    const i32_t rhs = static_cast<i32_t>(my_rhs);

    if (!AreSame(my_lhs, lhs) || !AreSame(my_rhs, rhs)) {
        error("Cannot use operator ^ for floats");
    }

    this->value = lhs ^ rhs;
}