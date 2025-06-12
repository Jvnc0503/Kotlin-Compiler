cmake_minimum_required(VERSION 3.30)
project(src)

set(CMAKE_CXX_STANDARD 23)

add_executable(src main.cpp
        token.h
        token.cpp
        scanner.h
        scanner.cpp)