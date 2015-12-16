#include <iostream>
#include "Expr.hpp"
#include "Eval.hpp"
#include "unique.hpp"

int main() {
    auto arr = std::make_unique<ArrayExpr>();
    arr->add(new NumberExpr(1));
    arr->add(new NumberExpr(2));
    arr->add(new NumberExpr(3));
    arr->add(new NumberExpr(4));

    auto e1 = std::make_unique<IndexAssignExpr>(arr.get(),
                                                new NumberExpr(1),
                                                new NumberExpr(42));
    auto e2 = std::make_unique<IndexAssignExpr>(arr.get(),
                                                new AddExpr(new NumberExpr(1), new NumberExpr(2)),
                                                new NumberExpr(23));

    Eval e;

    e1->accept(&e);
    for (auto& item : arr->expressions) {
        NumberExpr* nr = static_cast<NumberExpr*>(item.get());
        writeln(nr->value);
    }

    writeln("--------");

    e2->accept(&e);
    for (auto& item : arr->expressions) {
        NumberExpr* nr = static_cast<NumberExpr*>(item.get());
        writeln(nr->value);
    }
}