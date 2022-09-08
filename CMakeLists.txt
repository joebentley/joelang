cmake_minimum_required(VERSION 3.23)
project(joelang)

set(CMAKE_CXX_STANDARD 20)

add_executable(joelang main.cpp Lexer.cpp Lexer.h Token.h Token.cpp Parser.cpp Parser.h Expression.cpp Expression.h Literal.h)

add_subdirectory(JK)
target_link_libraries(joelang PUBLIC JK)
