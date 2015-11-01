//
// Created by Randy on 31.10.2015.
//

#include "Scope.hpp"
#include "Declaration.hpp"
#include "Expression.hpp"

Scope::Scope(Scope* sc) : _predecessor(sc) { }

Scope* Scope::restore() {
    return _predecessor.release();
}

const Scope* Scope::getPredecessor() const {
    return _predecessor.get();
}

void Scope::addVariable(VarDecl* vd) {
    _variables.emplace_back(vd);
}

VarDecl* Scope::findVariable(const std::string& name) const {
    if (!name.empty()) {
        const Scope* sc = this;

        do {
            for (auto it = _variables.begin(); it != _variables.end(); it++) {
                if ((*it)->getName() == name)
                    return it->get();
            }

            sc = sc->getPredecessor();
        } while (sc);
    }

    return nullptr;
}