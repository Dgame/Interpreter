#ifndef MATCH_HPP
#define MATCH_HPP

enum class Match {
    Exact,
    Convert,
    No
};

enum class Type {
    None,
    Integer,
    Decimal,
    String,
    Character,
    Boolean
};

bool isNumeric(Type);
Match match(Type, Type);

#endif