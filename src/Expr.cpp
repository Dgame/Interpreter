#include "Expr.hpp"
#include "Visitor.hpp"

NumberExpr::NumberExpr(float val) : value(val) { }

NumericExpr* NumberExpr::syntaxCopy() {
    return new NumberExpr(this->value);
}

void NumberExpr::accept(Visitor* v) {
    v->visit(this);
}


CharExpr::CharExpr(char val) : value(val) { }

NumericExpr* CharExpr::syntaxCopy() {
    return new CharExpr(this->value);
}

void CharExpr::accept(Visitor* v) {
    v->visit(this);
}


template <typename T>
BinaryExpr<T>::BinaryExpr(T* lhs, T* rhs) : left(lhs), right(rhs) { }


BinaryNumericExpr::BinaryNumericExpr(NumericExpr* lhs, NumericExpr* rhs) : BinaryExpr(lhs, rhs) { }


AddExpr::AddExpr(NumericExpr* lhs, NumericExpr* rhs) : BinaryNumericExpr(lhs, rhs) { }

NumericExpr* AddExpr::syntaxCopy() {
    return new AddExpr(this->left->syntaxCopy(), this->right->syntaxCopy());
}

void AddExpr::accept(Visitor* v) {
    v->visit(this);
}


SubExpr::SubExpr(NumericExpr* lhs, NumericExpr* rhs) : BinaryNumericExpr(lhs, rhs) { }

NumericExpr* SubExpr::syntaxCopy() {
    return new SubExpr(this->left->syntaxCopy(), this->right->syntaxCopy());
}

void SubExpr::accept(Visitor* v) {
    v->visit(this);
}


MulExpr::MulExpr(NumericExpr* lhs, NumericExpr* rhs) : BinaryNumericExpr(lhs, rhs) { }

NumericExpr* MulExpr::syntaxCopy() {
    return new MulExpr(this->left->syntaxCopy(), this->right->syntaxCopy());
}

void MulExpr::accept(Visitor* v) {
    v->visit(this);
}


DivExpr::DivExpr(NumericExpr* lhs, NumericExpr* rhs) : BinaryNumericExpr(lhs, rhs) { }

NumericExpr* DivExpr::syntaxCopy() {
    return new DivExpr(this->left->syntaxCopy(), this->right->syntaxCopy());
}

void DivExpr::accept(Visitor* v) {
    v->visit(this);
}


template <typename T>
UnaryExpr<T>::UnaryExpr(T* e) : exp(e) { }


UnaryNumericExpr::UnaryNumericExpr(NumericExpr* e) : UnaryExpr(e) { }


NotExpr::NotExpr(NumericExpr* e) : UnaryNumericExpr(e) { }

NumericExpr* NotExpr::syntaxCopy() {
    return new NotExpr(this->exp->syntaxCopy());
}

void NotExpr::accept(Visitor* v) {
    v->visit(this);
}


NegateExpr::NegateExpr(NumericExpr* e) : UnaryNumericExpr(e) { }

NumericExpr* NegateExpr::syntaxCopy() {
    return new NegateExpr(this->exp->syntaxCopy());
}

void NegateExpr::accept(Visitor* v) {
    v->visit(this);
}


void ArrayExpr::add(Expr* exp) {
    this->expressions.emplace_back(exp);
}

void ArrayExpr::setAt(int index, Expr* exp) {
    this->expressions.at(index).reset(exp);
}

Expr* ArrayExpr::getAt(int index) {
    return this->expressions.at(index).get();
}

AccessExpr* ArrayExpr::syntaxCopy() {
    ArrayExpr* exp = new ArrayExpr();
    for (auto& item : this->expressions) {
        exp->add(item->syntaxCopy());
    }

    return exp;
}

void ArrayExpr::accept(Visitor* v) {
    v->visit(this);
}


IndexAssignExpr::IndexAssignExpr(AccessExpr* arr, NumericExpr* idx, Expr* val) : array(arr), index(idx),
                                                                                 assignment(val) { }

IndexAssignExpr* IndexAssignExpr::syntaxCopy() {
    return new IndexAssignExpr(this->array, this->index->syntaxCopy(), this->assignment->syntaxCopy());
}

void IndexAssignExpr::accept(Visitor* v) {
    v->visit(this);
}