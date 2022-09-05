#pragma once

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

struct ParseError {
    std::string message;

    [[nodiscard]] std::string string() const { return message; }
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

    [[nodiscard]] std::variant<std::unique_ptr<Expression>, ParseError> parse();

private:
    [[nodiscard]] std::variant<std::unique_ptr<Expression>, ParseError> expression();
    [[nodiscard]] std::variant<std::unique_ptr<Expression>, ParseError> equality();
    [[nodiscard]] std::variant<std::unique_ptr<Expression>, ParseError> comparison();
    [[nodiscard]] std::variant<std::unique_ptr<Expression>, ParseError> term();
    [[nodiscard]] std::variant<std::unique_ptr<Expression>, ParseError> factor();
    [[nodiscard]] std::variant<std::unique_ptr<Expression>, ParseError> unary();
    [[nodiscard]] std::variant<std::unique_ptr<Expression>, ParseError> primary();
    bool accept(TokenType type);

    size_t current_token_index = 0;
    std::vector<Token> tokens;
};
