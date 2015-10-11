OBJS = main.cpp Visitor.cpp Expression.cpp Declaration.cpp Location.cpp Interpreter.cpp Token.cpp
CC = g++

COMPILER_WARNINGS = -Wall -Wnon-virtual-dtor -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wmain -Wzero-as-null-pointer-constant -Wextra -pedantic -Wfatal-errors -Wno-switch
COMPILER_FLAGS = -std=c++1y
COMPILER_FLAGS_DEBUG = -g
COMPILER_FLAGS_RELEASE = -O3

OBJ_NAME = main

all:
	$(CC) $(OBJS) $(COMPILER_WARNINGS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
debug:
	$(CC) $(OBJS) $(COMPILER_WARNINGS) $(COMPILER_FLAGS) $(COMPILER_FLAGS_DEBUG) $(LINKER_FLAGS) -o $(OBJ_NAME)
release:
	$(CC) $(OBJS) $(COMPILER_WARNINGS) $(COMPILER_FLAGS) $(COMPILER_FLAGS_RELEASE) $(LINKER_FLAGS) -o $(OBJ_NAME)