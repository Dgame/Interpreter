cmake_minimum_required(VERSION 3.3)
project(Interpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    include/Visitor/Eval.hpp
    include/Visitor/Index.hpp
    include/Visitor/Output.hpp
    include/Visitor/Print.hpp
    include/Visitor/Visitor.hpp
    include/Cursor.hpp
    include/Declaration.hpp
    include/error.hpp
    include/Expression.hpp
    include/Interpreter.hpp
    include/Lexer.hpp
    include/Location.hpp
    include/Scope.hpp
    include/Tok.hpp
    include/Token.hpp
    include/types.hpp
    include/unique.hpp
    src/Visitor/Eval.cpp
    src/Visitor/Index.cpp
    src/Visitor/Output.cpp
    src/Visitor/Print.cpp
    src/Declaration.cpp
    src/Expression.cpp
    src/Interpreter.cpp
    src/Lexer.cpp
    src/Location.cpp
    src/Scope.cpp
    src/Token.cpp
    main.cpp)

add_executable(Interpreter ${SOURCE_FILES})