//
// Created by Randy on 31.10.2015.
//

#include "Eval.hpp"
#include "Index.hpp"
#include "Expression.hpp"

#include <limits>

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
    const f32_t lhs = this->value;

    bae->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = static_cast<i32_t>(lhs) & static_cast<i32_t>(rhs);
}

void Eval::visit(const BitOrExpr* boe) {
    boe->lhs->accept(this);
    const f32_t lhs = this->value;

    boe->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = static_cast<i32_t>(lhs) | static_cast<i32_t>(rhs);
}

void Eval::visit(const BitNotExpr* bne) {
    bne->exp->accept(this);

    this->value = ~static_cast<i32_t>(this->value);
}

void Eval::visit(const BitXorExpr* bxe) {
    bxe->lhs->accept(this);
    const f32_t lhs = this->value;

    bxe->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = static_cast<i32_t>(lhs) ^ static_cast<i32_t>(rhs);
}

void Eval::visit(const AndExpr* ande) {
    ande->lhs->accept(this);
    const f32_t lhs = this->value;

    ande->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = lhs && rhs;
}

void Eval::visit(const OrExpr* ore) {
    ore->lhs->accept(this);
    const f32_t lhs = this->value;

    ore->rhs->accept(this);
    const f32_t rhs = this->value;

    this->value = lhs || rhs;
}

void Eval::visit(const CompareExpr* cmpe) {
    cmpe->lhs->accept(this);
    const f32_t lhs = this->value;

    cmpe->rhs->accept(this);
    const f32_t rhs = this->value;

    auto feq = [](f32_t a, f32_t b) {
        return std::fabs(a - b) < std::numeric_limits<f32_t>::epsilon();
    };

    switch (cmpe->cmp) {
        case Compare::Equal:
            this->value = feq(lhs, rhs);
            break;
        case Compare::NotEqual:
            this->value = !feq(lhs, rhs);
            break;
        case Compare::Greater:
            this->value = lhs > rhs;
            break;
        case Compare::GreaterOrEqual:
            this->value = lhs > rhs || feq(lhs, rhs);
            break;
        case Compare::Lower:
            this->value = lhs < rhs;
            break;
        case Compare::LowerOrEqual:
            this->value = lhs < rhs || feq(lhs, rhs);
            break;
        default:
            error("Unknown comparison");
    }
}