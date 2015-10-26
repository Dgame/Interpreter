OBJS = main.cpp \
       src\Expression.cpp \
       src\Declaration.cpp \
       src\Location.cpp \
       src\Interpreter.cpp \
       src\Token.cpp \
       src\Scope.cpp \
       src\Lexer.cpp \
       src\Visitor\EvalVisitor.cpp \
       src\Visitor\PrintVisitor.cpp \
       src\Visitor\OutputVisitor.cpp \
       src\Visitor\IndexVisitor.cpp

CC = g++

COMPILER_WARNINGS = -Wall \
                    -Wnon-virtual-dtor \
                    -Wshadow \
                    -Winit-self \
                    -Wredundant-decls \
                    -Wcast-align \
                    -Wundef \
                    -Wfloat-equal \
                    -Winline \
                    -Wunreachable-code \
                    -Wmissing-declarations \
                    -Wmissing-include-dirs \
                    -Wmain \
                    -Wzero-as-null-pointer-constant \
                    -Wextra \
                    -pedantic \
                    -Wfatal-errors \
                    -Wno-switch

COMPILER_FLAGS = -std=c++1y
COMPILER_FLAGS_DEBUG = -g
COMPILER_FLAGS_RELEASE = -O3

OBJ_NAME = main

all:
	$(CC) $(OBJS) $(COMPILER_WARNINGS) $(COMPILER_FLAGS) -Iinclude -Iinclude/Visitor $(LINKER_FLAGS) -o $(OBJ_NAME)
debug:
	$(CC) $(OBJS) $(COMPILER_WARNINGS) $(COMPILER_FLAGS) -Iinclude -Iinclude/Visitor $(COMPILER_FLAGS_DEBUG) $(LINKER_FLAGS) -o $(OBJ_NAME)
release:
	$(CC) $(OBJS) $(COMPILER_WARNINGS) $(COMPILER_FLAGS) -Iinclude -Iinclude/Visitor $(COMPILER_FLAGS_RELEASE) $(LINKER_FLAGS) -o $(OBJ_NAME)