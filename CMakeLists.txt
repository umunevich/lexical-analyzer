cmake_minimum_required(VERSION 4.0)
project(LexicalAnalysis)

set(CMAKE_CXX_STANDARD 20)

add_executable(LexicalAnalysis main.cpp
        Lexer.cpp
        Lexer.h
)
