cmake_minimum_required(VERSION 3.30)
project(src)

set(CMAKE_CXX_STANDARD 20)

add_executable(src main.cpp
        scanner/scanner.h
        scanner/scanner.cpp
        scanner/token.h
        scanner/token.cpp
        exp/exp.h
        exp/exp.cpp
        parser/parser.h
        parser/parser.cpp
        parser/parseExpressions.cpp
        parser/parseDeclarations.cpp
        parser/parseStatements.cpp
        visitor/declaration.h
        visitor/environment.h
        visitor/visitor.h
        visitor/visitor.cpp
        visitor/printvisitor.cpp
        visitor/typevisitor.cpp
        visitor/gencodevisitor.cpp)