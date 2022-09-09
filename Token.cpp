#include "Token.h"
#include <JK/Format.h>
#include <vector>

std::string token_type_string(TokenType type)
{
    switch (type) {
        case TokenType::string:
            return "string";
        case TokenType::number:
            return "number";
        case TokenType::plus:
            return "plus";
        case TokenType::minus:
            return "minus";
        case TokenType::asterisk:
            return "asterisk";
        case TokenType::slash:
            return "slash";
        case TokenType::percent:
            return "percent";
        case TokenType::chevron:
            return "chevron";
        case TokenType::angle_right:
            return "angle_right";
        case TokenType::angle_left:
            return "angle_left";
        case TokenType::angle_right_equals:
            return "angle_right_equals";
        case TokenType::angle_left_equals:
            return "angle_left_equals";
        case TokenType::ampersand_ampersand:
            return "ampersand_ampersand";
        case TokenType::pipe_pipe:
            return "pipe_pipe";
        case TokenType::equals_equals:
            return "equals_equals";
        case TokenType::bang_equals:
            return "bang_equals";
        case TokenType::bang:
            return "bang";
        case TokenType::left_paren:
            return "left_paren";
        case TokenType::right_paren:
            return "right_paren";
        case TokenType::true_tok:
            return "true_tok";
        case TokenType::false_tok:
            return "false_tok";
        default:
            assert(false);
    }
}

std::string token_type_lexeme(TokenType type)
{
    switch (type) {
        case TokenType::string:
            return "string";
        case TokenType::number:
            return "number";
        case TokenType::plus:
            return "+";
        case TokenType::minus:
            return "-";
        case TokenType::asterisk:
            return "*";
        case TokenType::slash:
            return "/";
        case TokenType::percent:
            return "%";
        case TokenType::chevron:
            return "^";
        case TokenType::angle_right:
            return ">";
        case TokenType::angle_left:
            return "<";
        case TokenType::angle_right_equals:
            return ">=";
        case TokenType::angle_left_equals:
            return "<=";
        case TokenType::ampersand_ampersand:
            return "&&";
        case TokenType::pipe_pipe:
            return "||";
        case TokenType::equals_equals:
            return "==";
        case TokenType::bang_equals:
            return "!=";
        case TokenType::bang:
            return "!";
        case TokenType::left_paren:
            return "(";
        case TokenType::right_paren:
            return ")";
        case TokenType::true_tok:
            return "true";
        case TokenType::false_tok:
            return "false";
        default:
            assert(false);
    }
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
