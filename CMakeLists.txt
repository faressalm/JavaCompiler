cmake_minimum_required(VERSION 3.23)
project(JavaCompiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(JavaCompiler main.cpp NFA/NFA.h NFA/NFA.cpp)
