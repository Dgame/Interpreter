//
// Created by Randy on 31.10.2015.
//

#include "Declaration.hpp"
#include "Expression.hpp"
#include "Visitor/Eval.hpp"
#include "Visitor/Print.hpp"
#include "Visitor/Output.hpp"

VarDecl::VarDecl(const std::string& name, Expr* exp, bool constant) : _isConst(constant), _name(name), _exp(exp) { }

void VarDecl::assign(Expr* e) {
    _exp.reset(e);
}

void VarDecl::assignAt(Expr* idx_exp, Expr* item_exp) {
    Eval ev(idx_exp);
    const u32_t index = static_cast<u32_t>(ev.value);

    Expr* exp = _exp.get();

    if (ArrayExpr* ae = dynamic_cast<ArrayExpr*>(exp))
        ae->exps.at(index).reset(item_exp);
    else
        error("Can only assign to an Array");
}

void VarDecl::append(Expr* item_exp) {
    Expr* exp = _exp.get();

    if (ArrayExpr* ae = dynamic_cast<ArrayExpr*>(exp))
        ae->exps.emplace_back(item_exp);
    else
        error("Can only append to an Array");
}

std::ostream& VarDecl::print(std::ostream& out) const {
    out << '<' << _name << '>' << ':';

    Print pv(out);
    _exp->accept(&pv);

    return out;
}

PrintDecl::PrintDecl(std::ostream& out) : _out(out) { }

void PrintDecl::add(Expr* exp) {
    _exps.emplace_back(exp);
}

std::ostream& PrintDecl::print(std::ostream& out) const {
    Print pv(out);

    out << "print ";
    for (auto& exp : _exps) {
        exp->accept(&pv);
    }

    return out;
}

void PrintDecl::eval() const {
    for (auto& exp : _exps) {
        Output(exp.get(), _out);
        // _out << ' ';
    }

    _out << std::endl;
}