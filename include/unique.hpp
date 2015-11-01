//
// Created by Randy on 31.10.2015.
//

#ifndef INTERPRETER_UNIQUE_HPP
#define INTERPRETER_UNIQUE_HPP

#include <memory>

namespace std {
    template <typename T, typename ...Args>
    std::unique_ptr<T> make_unique(Args&& ...args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

#endif //INTERPRETER_UNIQUE_HPP
