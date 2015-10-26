#ifndef INDEX_VISITOR_HPP
#define INDEX_VISITOR_HPP

#include "Visitor.hpp"
#include "types.hpp"

struct IndexVisitor : public Visitor {
    Visitor* _visitor;
    u32_t index = 0;

    explicit IndexVisitor(const IndexExpr*, Visitor*);

    virtual void visit(const ArrayExpr*);
    virtual void visit(const StringExpr*);
};

#endif