#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "Token.h"
#include <JK/Error.h>

using LexErrorOr = ErrorOr<std::vector<Token>>;

class Lexer {
public:
    explicit Lexer(std::string_view source) : source(source) {}

    LexErrorOr lex();

    void reset();

private:
    std::string_view source;
    int line = 1, column = 1;
    std::string_view::size_type index = 0;
    std::vector<Token> tokens;

    bool lex_token(TokenType token);
    std::optional<Error> convert_and_push_number(const std::string &literal);
};
