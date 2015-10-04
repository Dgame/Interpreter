#include "Variable.hpp"

Variable::Variable(const std::string& id, bool constant) : name(id), isConst(constant) { }

std::ostream& operator <<(std::ostream& out, Variable& var) {
    out << var.val;

    return out;
}