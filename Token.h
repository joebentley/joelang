#pragma once

#include "Literal.h"
#include <optional>
#include <string>
#include <utility>

enum class TokenType {
    string,
    number,
    plus,
    minus,
    asterisk,
    slash,
    angle_right,
    angle_left,
    angle_right_equals,
    angle_left_equals,
    equals_equals,
    bang_equals,
    bang,
    left_paren,
    right_paren,
    true_tok,
    false_tok
};

std::string token_type_string(TokenType type);
std::string token_type_lexeme(TokenType type);

struct Token {
    Token(TokenType type, int line, int column, std::string lexeme) : type(type), line(line), column(column), lexeme(std::move(lexeme)) {}

    Token(TokenType type, int line, int column, std::string lexeme, Literal literal) : type(type), line(line), column(column), lexeme(std::move(lexeme)), literal(std::move(literal)) {}

    TokenType type;
    int line;
    int column;
    std::string lexeme;
    std::optional<Literal> literal;

    [[nodiscard]] std::string string() const;
};
