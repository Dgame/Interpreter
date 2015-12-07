#include "Math.hpp"
#include "Expr.hpp"

void Math::visit(NumberExpr* exp) {
    this->value = exp->asFloat();
}

void Math::visit(CharExpr* exp) {
    this->value = exp->asFloat();
}

void Math::visit(NotExpr* exp) {
    exp->exp->accept(this);

    this->value = !this->value;
}

void Math::visit(NegateExpr* exp) {
    exp->exp->accept(this);

    this->value = this->value * -1;
}

void Math::visit(AddExpr* exp) {
    exp->left->accept(this);
    const float lhs = this->value;
    exp->right->accept(this);
    const float rhs = this->value;

    this->value = lhs + rhs;
}

void Math::visit(SubExpr* exp) {
    exp->left->accept(this);
    const float lhs = this->value;
    exp->right->accept(this);
    const float rhs = this->value;

    this->value = lhs - rhs;
}

void Math::visit(MulExpr* exp) {
    exp->left->accept(this);
    const float lhs = this->value;
    exp->right->accept(this);
    const float rhs = this->value;

    this->value = lhs * rhs;
}

void Math::visit(DivExpr* exp) {
    exp->left->accept(this);
    const float lhs = this->value;
    exp->right->accept(this);
    const float rhs = this->value;

    this->value = lhs / rhs;
}
