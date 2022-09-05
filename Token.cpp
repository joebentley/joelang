#include "Token.h"
#include <vector>

const std::vector<std::string> token_names = {
        "number",
        "plus",
        "minus",
        "asterisk",
        "slash",
        "angle_right",
        "angle_left",
        "angle_right_equals",
        "angle_left_equals",
        "equals_equals",
        "bang_equals",
        "bang",
        "left_paren",
        "right_paren",
        "true",
        "false"};

std::string token_type_string(TokenType type)
{
    return token_names[static_cast<int>(type)];
}

const std::vector<std::string> token_lexemes = {
        "number",
        "+",
        "-",
        "*",
        "/",
        ">",
        "<",
        ">=",
        "<=",
        "==",
        "!=",
        "!",
        "(",
        ")",
        "true",
        "false"};

std::string token_type_lexeme(TokenType type)
{
    return token_lexemes[static_cast<int>(type)];
}

std::string Token::string() const
{
    std::string _literal;
    if (literal.has_value()) {
        _literal = " " + std::to_string(std::any_cast<double>(literal));
    }

    return std::to_string(line) + ":" + std::to_string(column) + " " + token_type_string(type) + " " + lexeme + _literal;
}
