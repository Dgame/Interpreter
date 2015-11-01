//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_SCOPE_HPP
#define INTERPRETER_SCOPE_HPP

#include <memory>
#include <vector>

class VarDecl;

class Scope {
private:
    std::unique_ptr<Scope> _predecessor;
    std::vector<std::unique_ptr<VarDecl>> _variables;

public:
    Scope() = default;
    explicit Scope(Scope*);

    Scope* restore();
    const Scope* getPredecessor() const;

    void addVariable(VarDecl*);
    VarDecl* findVariable(const std::string&) const;
};

#endif //INTERPRETER_SCOPE_HPP
