#include "match.hpp"

bool isNumeric(Type type) {
    return type == Type::Integer || type == Type::Decimal;
}

Match match(Type lhs, Type rhs) {
    if (rhs == lhs) {
        return Match::Exact;
    }

    if (isNumeric(lhs) && isNumeric(rhs)) {
        return Match::Convert;
    }

    return Match::No;
}