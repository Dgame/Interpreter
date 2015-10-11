#include "Expression.hpp"
#include "Visitor.hpp"

VarExpr::VarExpr(const Expr* e) : exp(e) { }

void VarExpr::accept(Visitor* v) const {
    v->visit(this);
}

void ArrayExpr::add(Expr* exp) {
    this->exps.emplace_back(exp);
}

void ArrayExpr::accept(Visitor* v) const {
    v->visit(this);
}

IndexExpr::IndexExpr(const Expr* e, Expr* i) : exp(e), index(i) { }

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

CharExpr::CharExpr(char c) : value(c) { }

void CharExpr::accept(Visitor* v) const {
     v->visit(this);
 }

StringExpr::StringExpr(const std::string& val) : value(val) { }

void StringExpr::accept(Visitor* v) const {
    v->visit(this);
}

UnaExpr::UnaExpr(Expr* e) : exp(e) { }

NegExpr::NegExpr(Expr* e) : UnaExpr(e) { }

void NegExpr::accept(Visitor* v) const {
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