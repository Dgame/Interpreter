//
// Created by Randy on 31.10.2015.
//

#include "Expression.hpp"
#include "Visitor.hpp"
#include "Declaration.hpp"

void NullExpr::accept(Visitor* v) const {
    v->visit(this);
}

VarExpr::VarExpr(const VarDecl* vd) : exp(vd->getExpr()) { }

void VarExpr::accept(Visitor* v) const {
    v->visit(this);
}

void ArrayExpr::add(Expr* exp) {
    this->exps.emplace_back(exp);
}

void ArrayExpr::accept(Visitor* v) const {
    v->visit(this);
}

IndexExpr::IndexExpr(const VarDecl* vd, Expr* i) : VarExpr(vd), index(i) { }

void IndexExpr::accept(Visitor* v) const {
    v->visit(this);
}

IntExpr::IntExpr(i32_t val) : value(val) { }

void IntExpr::accept(Visitor* v) const {
    v->visit(this);;
}

FloatExpr::FloatExpr(f32_t val) : value(val) { }

void FloatExpr::accept(Visitor* v) const {
    v->visit(this);
}

BoolExpr::BoolExpr(bool val) : value(val) { }

void BoolExpr::accept(Visitor* v) const {
    v->visit(this);
}

CharExpr::CharExpr(char val) : value(val) { }

void CharExpr::accept(Visitor* v) const {
    v->visit(this);
}

StringExpr::StringExpr(const std::string& val) : value(val) { }

void StringExpr::accept(Visitor* v) const {
    v->visit(this);
}

UnaExpr::UnaExpr(Expr* e) : exp(e) { }

NegationExpr::NegationExpr(Expr* e) : UnaExpr(e) { }

void NegationExpr::accept(Visitor* v) const {
    v->visit(this);
}

NotExpr::NotExpr(Expr* e) : UnaExpr(e) { }

void NotExpr::accept(Visitor* v) const {
    v->visit(this);
}

ParenExpr::ParenExpr(Expr* e) : UnaExpr(e) { }

void ParenExpr::accept(Visitor* v) const {
    v->visit(this);
}

BinExpr::BinExpr(Expr* l, Expr* r) : lhs(l), rhs(r) { }

AddExpr::AddExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

void AddExpr::accept(Visitor* v) const {
    v->visit(this);
}

SubExpr::SubExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

void SubExpr::accept(Visitor* v) const {
    v->visit(this);
}

MulExpr::MulExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

void MulExpr::accept(Visitor* v) const {
    v->visit(this);
}

DivExpr::DivExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

void DivExpr::accept(Visitor* v) const {
    v->visit(this);
}

ModExpr::ModExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

void ModExpr::accept(Visitor* v) const {
    v->visit(this);
}

BitAndExpr::BitAndExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

void BitAndExpr::accept(Visitor* v) const {
    v->visit(this);
}

BitOrExpr::BitOrExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

void BitOrExpr::accept(Visitor* v) const {
    v->visit(this);
}

BitNotExpr::BitNotExpr(Expr* e) : UnaExpr(e) { }

void BitNotExpr::accept(Visitor* v) const {
    v->visit(this);
}

BitXorExpr::BitXorExpr(Expr* l, Expr* r) : BinExpr(l, r) { }

void BitXorExpr::accept(Visitor* v) const {
    v->visit(this);
}