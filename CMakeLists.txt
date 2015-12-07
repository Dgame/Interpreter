cmake_minimum_required(VERSION 3.3)
project(Interpreter2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/Expr.cpp
        main.cpp
        src/Math.cpp
        src/Visitor.cpp)

include_directories("include")
add_executable(Interpreter2 ${SOURCE_FILES})