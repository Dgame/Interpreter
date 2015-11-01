//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_INDEX_HPP
#define INTERPRETER_INDEX_HPP

#include "Visitor.hpp"
#include "types.hpp"

struct Index : public Visitor {
    Visitor* _visitor;
    u32_t index = 0;

    explicit Index(const IndexExpr*, Visitor*);

    virtual void fail() {
        throw "Invalid Index Expression";
    }

    virtual void visit(const ArrayExpr*);
    virtual void visit(const StringExpr*);
};

#endif //INTERPRETER_INDEX_HPP
