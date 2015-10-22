#ifndef SCOPE_HPP
#define SCOPE_HPP

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

#endif