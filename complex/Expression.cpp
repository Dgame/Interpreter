#include "Expression.hpp"

VarExpr::VarExpr(Expr* e) : exp(e) { }

std::ostream& VarExpr::print(std::ostream& out) const {
    this->exp->print(out);

    return out;
}

IntExpr::IntExpr(i32_t i) : ival(i) { }

std::ostream& IntExpr::print(std::ostream& out) const {
    out << this->ival;

    return out;
}

FloatExpr::FloatExpr(f32_t d) : dval(d) { }

std::ostream& FloatExpr::print(std::ostream& out) const {
    out << this->dval;

    return out;
}

UnaExpr::UnaExpr(Expr* e) : exp(e) { }

NegExpr::NegExpr(Expr* e) : UnaExpr(e) { }

std::ostream& NegExpr::print(std::ostream& out) const {
    out << '-';
    this->exp->print(out);

    return out;
}

ParenExpr::ParenExpr(Expr* e) : UnaExpr(e) { }

std::ostream& ParenExpr::print(std::ostream& out) const {
    out << '(';
    this->exp->print(out);
    out << ')';

    return out;
}

BinExpr::BinExpr(Expr* l, Expr* r) : lhs(l), rhs(r) { }

AddExpr::AddExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

std::ostream& AddExpr::print(std::ostream& out) const {
    this->lhs->print(out);
    out << " + ";
    this->rhs->print(out);

    return out;
}

SubExpr::SubExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

std::ostream& SubExpr::print(std::ostream& out) const {
    this->lhs->print(out);
    out << " - ";
    this->rhs->print(out);

    return out;
}

MulExpr::MulExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

std::ostream& MulExpr::print(std::ostream& out) const {
    this->lhs->print(out);
    out << " * ";
    this->rhs->print(out);

    return out;
}

DivExpr::DivExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

std::ostream& DivExpr::print(std::ostream& out) const {
    this->lhs->print(out);
    out << " / ";
    this->rhs->print(out);

    return out;
}

ModExpr::ModExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

std::ostream& ModExpr::print(std::ostream& out) const {
    this->lhs->print(out);
    out << " % ";
    this->rhs->print(out);

    return out;
}