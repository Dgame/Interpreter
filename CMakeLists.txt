cmake_minimum_required(VERSION 3.3)
project(Interpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
                src/Token.cpp
                src/Location.cpp
                src/Lexer.cpp
                src/Expression.cpp
                src/Declaration.cpp
                src/Visitor/Eval.cpp
                src/Visitor/Index.cpp
                src/Visitor/Output.cpp
                src/Visitor/Print.cpp
                src/Scope.cpp
                src/Interpreter.cpp include/unique.hpp)

include_directories("include")
include_directories("include/Visitor")

add_executable(Interpreter ${SOURCE_FILES})