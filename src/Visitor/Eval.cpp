//
// Created by Randy on 31.10.2015.
//

#include "Eval.hpp"
#include "Index.hpp"
#include "Expression.hpp"

#include <limits>

inline bool AreSame(f32_t lhs, f32_t rhs) {
    return std::fabs(lhs - rhs) < std::numeric_limits<f32_t>::epsilon();
}

Eval::Eval(const Expr* exp) {
    exp->accept(this);
}

void Eval::visit(const VarExpr* ve) {
    ve->exp->accept(this);
}

void Eval::visit(const IndexExpr* ie) {
    Index(ie, this);
}

void Eval::visit(const IntExpr* ie) {
    this->value = ie->value;
}

void Eval::visit(const FloatExpr* fe) {
    this->value = fe->value;
}

void Eval::visit(const BoolExpr* be) {
    this->value = static_cast<i32_t>(be->value);
}

void Eval::visit(const NegationExpr* ne) {
    ne->exp->accept(this);
    this->value = this->value * -1;
}

void Eval::visit(const NotExpr* ne) {
    ne->exp->accept(this);
    this->value = !static_cast<i32_t>(this->value);
}

void Eval::visit(const ParenExpr* pe) {
    pe->exp->accept(this);
}

void Eval::visit(const AddExpr* add) {
    add->lhs->accept(this);
    const f32_t lhs = this->value;

    add->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = lhs + rhs;
}

void Eval::visit(const SubExpr* sub) {
    sub->lhs->accept(this);
    const f32_t lhs = this->value;

    sub->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = lhs - rhs;
}

void Eval::visit(const MulExpr* mul) {
    mul->lhs->accept(this);
    const f32_t lhs = this->value;

    mul->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = lhs * rhs;
}

void Eval::visit(const DivExpr* div) {
    div->lhs->accept(this);
    const f32_t lhs = this->value;

    div->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = lhs * rhs;
}

void Eval::visit(const ModExpr* mod) {
    mod->lhs->accept(this);
    const f32_t lhs = this->value;

    mod->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = std::fmod(lhs, rhs);
}

void Eval::visit(const BitAndExpr* bae) {
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

void Eval::visit(const BitOrExpr* boe) {
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

void Eval::visit(const BitNotExpr* bne) {
    bne->exp->accept(this);
    const f32_t my_lhs = this->value;

    const i32_t lhs = static_cast<i32_t>(my_lhs);

    if (!AreSame(my_lhs, lhs)) {
        error("Cannot use operator ~ for float");
    }

    this->value = ~lhs;
}

void Eval::visit(const BitXorExpr* bxe) {
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