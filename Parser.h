#pragma once

#include "Error.h"
#include "Expression.h"
#include "Token.h"
#include <memory>
#include <variant>
#include <vector>

// Stolen from "crafting interpreters" book
/*
expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
*/

using ParserErrorOr = ErrorOr<std::unique_ptr<Expression>>;

class Parser {
public:
    explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

    [[nodiscard]] ParserErrorOr parse();

private:
    [[nodiscard]] ParserErrorOr expression();
    [[nodiscard]] ParserErrorOr equality();
    [[nodiscard]] ParserErrorOr comparison();
    [[nodiscard]] ParserErrorOr term();
    [[nodiscard]] ParserErrorOr factor();
    [[nodiscard]] ParserErrorOr unary();
    [[nodiscard]] ParserErrorOr primary();
    bool accept(TokenType type);

    size_t current_token_index = 0;
    std::vector<Token> tokens;
};
