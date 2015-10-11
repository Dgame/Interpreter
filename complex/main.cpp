#include <ostream>
#include "Interpreter.hpp"
#include "Expression.hpp"
// #include "Visitor.hpp"

int main() {
    Interpreter ip;

    try {
        ip.parse("test.txt");
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

    // std::unique_ptr<ArrayExpr> arr(new ArrayExpr());
    // arr->add(new IntExpr(1));
    // arr->add(new IntExpr(2));
    // arr->add(new IntExpr(3));

    // std::unique_ptr<IndexExpr> ie(new IndexExpr(arr.get(), new IntExpr(4)));

    // PrintVisitor pv(std::cout);
    // ie->accept(&pv);
}