#include "OutputVisitor.hpp"
#include "EvalVisitor.hpp"
#include "IndexVisitor.hpp"
#include "Expression.hpp"
#include "types.hpp"

OutputVisitor::OutputVisitor(const Expr* exp, std::ostream& out) : _out(out) {
    exp->accept(this);
}

void OutputVisitor::visit(const NullExpr*) {
    _out << "null";
}

void OutputVisitor::visit(const VarExpr* ve) {
    ve->exp->accept(this);
}

void OutputVisitor::visit(const ArrayExpr* ae) {
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

void OutputVisitor::visit(const IndexExpr* ie) {
    IndexVisitor(ie, this);
}

void OutputVisitor::visit(const IntExpr* ie) {
    _out << ie->value;
}

void OutputVisitor::visit(const FloatExpr* fe) {
    _out << fe->value;
}

void OutputVisitor::visit(const BoolExpr* be) {
    _out << (be->value ? "true" : "false");
}

void OutputVisitor::visit(const CharExpr* ce) {
    _out << ce->value;
}

void OutputVisitor::visit(const StringExpr* se) {
    _out << se->value;
}

void OutputVisitor::visit(const NegationExpr* ne) {
    EvalVisitor ev;
    ne->exp->accept(&ev);

    _out << ev.value * -1;
}

void OutputVisitor::visit(const NotExpr* ne) {
    EvalVisitor ev;
    ne->exp->accept(&ev);

    _out << !static_cast<i32_t>(ev.value);
}

void OutputVisitor::visit(const ParenExpr* pe) {
    pe->exp->accept(this);
}

void OutputVisitor::visit(const AddExpr* add) {
    EvalVisitor ev(add);

    _out << ev.value;
}

void OutputVisitor::visit(const SubExpr* sub) {
    EvalVisitor ev(sub);

    _out << ev.value;
}

void OutputVisitor::visit(const MulExpr* mul) {
    EvalVisitor ev(mul);

    _out << ev.value;
}

void OutputVisitor::visit(const DivExpr* div) {
    EvalVisitor ev(div);

    _out << ev.value;
}

void OutputVisitor::visit(const ModExpr* mod) {
    EvalVisitor ev(mod);

    _out << ev.value;
}

void OutputVisitor::visit(const BitAndExpr* bae) {
    EvalVisitor ev(bae);

    _out << ev.value;
}

void OutputVisitor::visit(const BitOrExpr* boe) {
    EvalVisitor ev(boe);

    _out << ev.value;
}

void OutputVisitor::visit(const BitNotExpr* bne) {
    EvalVisitor ev(bne);

    _out << ev.value;
}

void OutputVisitor::visit(const BitXorExpr* bxe) {
    EvalVisitor ev(bxe);

    _out << ev.value;
}