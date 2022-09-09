#pragma once

#include "Expression.h"
#include "Token.h"
#include <JK/Error.h>
#include <memory>
#include <variant>
#include <vector>

// Stolen from "crafting interpreters" book
/*
expression     → boolean_or ;
boolean_or     → equality ( "||" equality )* ;
boolean_and    → equality ( "&&" equality )* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → exponent ( ( "/" | "*" | "%" ) exponent )* ;
exponent       → unary ( "^" unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
*/

using ParserErrorOr = ErrorOr<std::unique_ptr<Expression>>;

// There are two times that "current_token_index" is incremented:
// when accept() matches the given token with the current one
// and when primary() sees a pure literal
class Parser {
public:
    explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

    [[nodiscard]] ParserErrorOr parse();

private:
    [[nodiscard]] ParserErrorOr expression();
    [[nodiscard]] ParserErrorOr boolean_or();
    [[nodiscard]] ParserErrorOr boolean_and();
    [[nodiscard]] ParserErrorOr equality();
    [[nodiscard]] ParserErrorOr comparison();
    [[nodiscard]] ParserErrorOr term();
    [[nodiscard]] ParserErrorOr factor();
    [[nodiscard]] ParserErrorOr exponent();
    [[nodiscard]] ParserErrorOr unary();
    [[nodiscard]] ParserErrorOr primary();
    bool accept(TokenType type);

    size_t current_token_index = 0;
    std::vector<Token> tokens;
};
