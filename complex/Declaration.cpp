#include "Declaration.hpp"
#include "Expression.hpp"

VarDecl::VarDecl(const std::string& id, Expr* e) : name(id), exp(e) { }

std::ostream& VarDecl::print(std::ostream& out) const {
    out << '<' << this->name << '>' << ':';
    this->exp->print(out);

    return out;
}

f32_t VarDecl::eval() const {
    return this->exp->eval();
}

void PrintDecl::add(Expr* exp) {
    this->exps.emplace_back(exp);
}

std::ostream& PrintDecl::print(std::ostream& out) const {
    out << "print ";
    for (auto& exp : this->exps) {
        exp->print(out);
    }

    return out;
}

f32_t PrintDecl::eval() const {
    for (auto& exp : this->exps) {
        std::cout << exp->eval() << ' ';
    }

    std::cout << std::endl;

    return 0;
}