#include "Declaration.hpp"
#include "Expression.hpp"
#include "Visitor.hpp"

VarDecl::VarDecl(const std::string& id, Expr* e) : name(id), exp(e) { }

std::ostream& VarDecl::print(std::ostream& out) const {
    out << '<' << this->name << '>' << ':';

    PrintVisitor pv(out);
    this->exp->accept(&pv);

    return out;
}

void VarDecl::eval() const {
    EvalVisitor ev;
    this->exp->accept(&ev);
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
    EvalVisitor ev;
    PrintVisitor pv(std::cout);

    for (auto& exp : this->exps) {
        if (exp->needEvaluation()) {
            exp->accept(&ev);
            std::cout << ev.value;
        } else {
           exp->accept(&pv);
        }
        std::cout << ' ';
    }

    std::cout << std::endl;
}