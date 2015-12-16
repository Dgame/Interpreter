cmake_minimum_required(VERSION 3.3)
project(Interpreter2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        include/Eval.hpp
        include/Expr.hpp
        include/Math.hpp
        include/types.hpp
        include/unique.hpp
        include/util.hpp
        include/Visitor.hpp
        src/Expr.cpp
        main.cpp
        src/Math.cpp
        src/Eval.cpp)

include_directories("include")
add_executable(Interpreter2 ${SOURCE_FILES})