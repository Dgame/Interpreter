//
// Created by Randy on 31.10.2015.
//

#include "Output.hpp"
#include "Eval.hpp"
#include "Index.hpp"
#include "Expression.hpp"
#include "types.hpp"

Output::Output(const Expr* exp, std::ostream& out) : _out(out) {
    exp->accept(this);
}

void Output::visit(const NullExpr*) {
    _out << "null";
}

void Output::visit(const VarExpr* ve) {
    ve->exp->accept(this);
}

void Output::visit(const ArrayExpr* ae) {
    u32_t i = 0;

    _out << '[';
    for (auto& exp : ae->exps) {
        if (i != 0)
            _out << ',';

        exp->accept(this);

        i++;
    }
    _out << ']';
}

void Output::visit(const IndexExpr* ie) {
    Index(ie, this);
}

void Output::visit(const IntExpr* ie) {
    _out << ie->value;
}

void Output::visit(const FloatExpr* fe) {
    _out << fe->value;
}

void Output::visit(const BoolExpr* be) {
    _out << (be->value ? "true" : "false");
}

void Output::visit(const CharExpr* ce) {
    _out << ce->value;
}

void Output::visit(const StringExpr* se) {
    _out << se->value;
}

void Output::visit(const NegationExpr* ne) {
    Eval ev;
    ne->exp->accept(&ev);

    _out << ev.value * -1;
}

void Output::visit(const NotExpr* ne) {
    Eval ev;
    ne->exp->accept(&ev);

    _out << !static_cast<i32_t>(ev.value);
}

void Output::visit(const ParenExpr* pe) {
    pe->exp->accept(this);
}

void Output::visit(const AddExpr* add) {
    Eval ev(add);

    _out << ev.value;
}

void Output::visit(const SubExpr* sub) {
    Eval ev(sub);

    _out << ev.value;
}

void Output::visit(const MulExpr* mul) {
    Eval ev(mul);

    _out << ev.value;
}

void Output::visit(const DivExpr* div) {
    Eval ev(div);

    _out << ev.value;
}

void Output::visit(const ModExpr* mod) {
    Eval ev(mod);

    _out << ev.value;
}

void Output::visit(const BitAndExpr* bae) {
    Eval ev(bae);

    _out << ev.value;
}

void Output::visit(const BitOrExpr* boe) {
    Eval ev(boe);

    _out << ev.value;
}

void Output::visit(const BitNotExpr* bne) {
    Eval ev(bne);

    _out << ev.value;
}

void Output::visit(const BitXorExpr* bxe) {
    Eval ev(bxe);

    _out << ev.value;
}