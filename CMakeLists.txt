cmake_minimum_required(VERSION 3.23)
project(joelang)

set(CMAKE_CXX_STANDARD 17)

add_executable(joelang main.cpp Lexer.cpp Lexer.h Token.h Token.cpp Parser.cpp Parser.h Expression.cpp Expression.h)
