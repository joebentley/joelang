#include "Token.h"
#include <JK/Format.h>
#include <vector>

const std::vector<std::string> token_names = {
        "string",
        "number",
        "plus",
        "minus",
        "asterisk",
        "slash",
        "percent",
        "chevron",
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
        "string",
        "number",
        "+",
        "-",
        "*",
        "/",
        "%",
        "^",
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
        if (std::holds_alternative<double>(*literal)) {
            _literal = " " + std::to_string(std::get<double>(*literal));
        } else if (std::holds_alternative<std::string>(*literal)) {
            _literal = " \"" + std::get<std::string>(*literal) + "\"";
        }
    }

    if (_literal.empty())
        return Format::fmt("{}: {} {} \"{}\"", std::to_string(line), std::to_string(column), token_type_string(type), lexeme);
    else
        return Format::fmt("{}: {} {} \"{}\" {}", std::to_string(line), std::to_string(column), token_type_string(type), lexeme, _literal);
}
