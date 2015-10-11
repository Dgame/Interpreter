#include "Declaration.hpp"
#include "Expression.hpp"
#include "Visitor.hpp"

VarDecl::VarDecl(const std::string& id, Expr* e) : name(id), exp(e) { }

void VarDecl::assign(Expr* e) {
    this->exp.reset(e);
}

std::ostream& VarDecl::print(std::ostream& out) const {
    out << '<' << this->name << '>' << ':';

    PrintVisitor pv(out);
    this->exp->accept(&pv);

    return out;
}

void PrintDecl::add(Expr* exp) {
    this->exps.emplace_back(exp);
}

std::ostream& PrintDecl::print(std::ostream& out) const {
    PrintVisitor pv(out);

    out << "print ";
    for (auto& exp : this->exps) {
        exp->accept(&pv);
    }

    return out;
}

void PrintDecl::eval() const {
    for (auto& exp : this->exps) {
        OutputVisitor(exp.get(), std::cout);

        std::cout << ' ';
    }

    std::cout << std::endl;
}