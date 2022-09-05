#include "Lexer.h"
#include <cctype>
#include <stdexcept>

Error lex_error(int line, int column, const std::string &message)
{
    return Error{"Error at " + std::to_string(line) + ":" + std::to_string(column) + " " + message};
}

std::optional<Error> Lexer::convert_and_push_number(const std::string &literal)
{
    size_t num_processed = 0;
    try {
        tokens.emplace_back(TokenType::number, line, column, literal, std::make_any<double>(std::stof(literal, &num_processed)));
    } catch (std::logic_error const &) {
        return lex_error(line, column, "could not lex number: " + literal);
    }
    if (num_processed != literal.size())
        return lex_error(line, column, "could not lex number: " + literal);
    return {};
}

bool Lexer::lex_token(TokenType token)
{
    std::string string = token_type_lexeme(token);
    for (auto i = 0; i < string.size(); i++) {
        if (source[index + i] != string[i] || index + i > source.size())
            return false;
    }
    index += string.size();
    tokens.emplace_back(token, line, column, token_type_lexeme(token));
    return true;
}

#define CONTINUE_IF_MATCH(token) \
    if (lex_token(token))        \
        continue;

LexErrorOr Lexer::lex()
{
    bool in_number = false;
    std::string literal;

    reset();

    while (index < source.size()) {
        char c = source[index];

        column++;

        // Number

        if ((c == '.' && std::isdigit(source[index + 1])) || std::isdigit(c)) {
            literal += c;
            in_number = true;
            index++;
            if (index == source.size()) {
                auto err = convert_and_push_number(literal);
                if (err.has_value())
                    return err.value();
            }
            continue;
        } else {
            if (in_number) {
                auto err = convert_and_push_number(literal);
                if (err.has_value())
                    return err.value();
                literal = "";
            }
            in_number = false;
        }

        // Keywords

        CONTINUE_IF_MATCH(TokenType::true_tok)
        CONTINUE_IF_MATCH(TokenType::false_tok)
        CONTINUE_IF_MATCH(TokenType::equals_equals)
        CONTINUE_IF_MATCH(TokenType::bang_equals)
        CONTINUE_IF_MATCH(TokenType::angle_left_equals)
        CONTINUE_IF_MATCH(TokenType::angle_right_equals)
        CONTINUE_IF_MATCH(TokenType::plus)
        CONTINUE_IF_MATCH(TokenType::minus)
        CONTINUE_IF_MATCH(TokenType::asterisk)
        CONTINUE_IF_MATCH(TokenType::slash)
        CONTINUE_IF_MATCH(TokenType::bang)
        CONTINUE_IF_MATCH(TokenType::angle_left)
        CONTINUE_IF_MATCH(TokenType::angle_right)
        CONTINUE_IF_MATCH(TokenType::left_paren)
        CONTINUE_IF_MATCH(TokenType::right_paren)

        if (c == '\r' && source[index + 1] != '\n') {
            return lex_error(line, column, "\\r that is not followed by \\n");
        }

        if (c == '\r' && source[index + 1] == '\n') {
            line++;
            column = 1;
            continue;
        } else if (std::isblank(c) || c == '\n') {
            if (c == '\n') {
                line++;
                column = 1;
            }
            index++;
            continue;
        }

        return lex_error(line, column, "could not match at " + std::string(1, c));
    }

    return tokens;
}

void Lexer::reset()
{
    tokens.clear();
    index = 0;
    line = 1;
    column = 1;
}
