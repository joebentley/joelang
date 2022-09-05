#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "Token.h"

struct LexError {
    int line;
    int column;
    std::string message;

    [[nodiscard]] std::string string() const;
};

class Lexer {
public:
    explicit Lexer(std::string_view source) : source(source) {}

    std::variant<std::vector<Token>, LexError> lex();

    void reset();

private:
    std::string_view source;
    int line = 1, column = 1;
    std::string_view::size_type index = 0;
    std::vector<Token> tokens;

    bool lex_token(TokenType token);
    std::optional<LexError> convert_and_push_number(const std::string &literal);
};

std::variant<std::vector<Token>, LexError> lex(std::string_view source);
