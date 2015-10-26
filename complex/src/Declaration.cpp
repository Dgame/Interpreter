#include "Declaration.hpp"
#include "Expression.hpp"
#include "Visitor/PrintVisitor.hpp"
#include "Visitor/OutputVisitor.hpp"

VarDecl::VarDecl(const std::string& name, Expr* exp, bool constant) : _name(name), _exp(exp), _isConst(constant) { }

const Expr* VarDecl::getExpr() const {
    return _exp.get();
}

void VarDecl::assign(Expr* e) {
    _exp.reset(e);
}

std::ostream& VarDecl::print(std::ostream& out) const {
    out << '<' << _name << '>' << ':';

    PrintVisitor pv(out);
    _exp->accept(&pv);

    return out;
}

void PrintDecl::add(Expr* exp) {
    _exps.emplace_back(exp);
}

std::ostream& PrintDecl::print(std::ostream& out) const {
    PrintVisitor pv(out);

    out << "print ";
    for (auto& exp : _exps) {
        exp->accept(&pv);
    }

    return out;
}

void PrintDecl::eval() const {
    for (auto& exp : _exps) {
        OutputVisitor(exp.get(), std::cout);
        // std::cout << ' ';
    }

    std::cout << std::endl;
}