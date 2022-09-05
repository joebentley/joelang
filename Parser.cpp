#include "Parser.h"

#define TRY(ident)                                 \
    if (std::holds_alternative<ParseError>(ident)) \
        return std::get<ParseError>(ident);

std::variant<std::unique_ptr<Expression>, ParseError> Parser::parse()
{
    auto maybe_expr = expression();
    TRY(maybe_expr)

    if (current_token_index < tokens.size()) {
        return ParseError{"Syntax error at token " + std::to_string(current_token_index) + "   " + tokens[current_token_index].string()};
    }

    return maybe_expr;
}

std::variant<std::unique_ptr<Expression>, ParseError> Parser::expression()// NOLINT(misc-no-recursion)
{
    return equality();
}

std::variant<std::unique_ptr<Expression>, ParseError> Parser::equality()// NOLINT(misc-no-recursion)
{
    auto maybe_expr = comparison();
    TRY(maybe_expr)

    auto expr = std::move(std::get<std::unique_ptr<Expression>>(maybe_expr));

    for (;;) {
        if (accept(TokenType::equals_equals)) {
            maybe_expr = comparison();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::equals, std::move(std::get<std::unique_ptr<Expression>>(maybe_expr)));
        } else if (accept(TokenType::bang_equals)) {
            maybe_expr = comparison();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::not_equals, std::move(std::get<std::unique_ptr<Expression>>(maybe_expr)));
        } else {
            return expr;
        }
    }
}

std::variant<std::unique_ptr<Expression>, ParseError> Parser::comparison()// NOLINT(misc-no-recursion)
{
    auto maybe_expr = term();
    TRY(maybe_expr)

    auto expr = std::move(std::get<std::unique_ptr<Expression>>(maybe_expr));

    for (;;) {
        if (accept(TokenType::angle_left)) {
            maybe_expr = term();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::less_than, std::move(std::get<std::unique_ptr<Expression>>(maybe_expr)));
        } else if (accept(TokenType::angle_right)) {
            maybe_expr = term();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::greater_than, std::move(std::get<std::unique_ptr<Expression>>(maybe_expr)));
        } else if (accept(TokenType::angle_left_equals)) {
            maybe_expr = term();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::less_than_equals, std::move(std::get<std::unique_ptr<Expression>>(maybe_expr)));
        } else if (accept(TokenType::angle_right_equals)) {
            maybe_expr = term();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::greater_than_equals, std::move(std::get<std::unique_ptr<Expression>>(maybe_expr)));
        } else {
            return expr;
        }
    }
}

std::variant<std::unique_ptr<Expression>, ParseError> Parser::term()// NOLINT(misc-no-recursion)
{
    auto maybe_expr = factor();
    TRY(maybe_expr)

    auto expr = std::move(std::get<std::unique_ptr<Expression>>(maybe_expr));

    for (;;) {
        if (accept(TokenType::plus)) {
            maybe_expr = factor();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::add, std::move(std::get<std::unique_ptr<Expression>>(maybe_expr)));
        } else if (accept(TokenType::minus)) {
            maybe_expr = factor();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::subtract, std::move(std::get<std::unique_ptr<Expression>>(maybe_expr)));
        } else {
            return expr;
        }
    }
}

std::variant<std::unique_ptr<Expression>, ParseError> Parser::factor()// NOLINT(misc-no-recursion)
{
    auto maybe_expr = unary();
    TRY(maybe_expr)

    auto expr = std::move(std::get<std::unique_ptr<Expression>>(maybe_expr));

    for (;;) {
        if (accept(TokenType::asterisk)) {
            maybe_expr = unary();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::multiply, std::move(std::get<std::unique_ptr<Expression>>(maybe_expr)));
        } else if (accept(TokenType::slash)) {
            maybe_expr = unary();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::divide, std::move(std::get<std::unique_ptr<Expression>>(maybe_expr)));
        } else {
            return expr;
        }
    }
}

std::variant<std::unique_ptr<Expression>, ParseError> Parser::unary()// NOLINT(misc-no-recursion)
{
    // TODO: boolean negation
    if (accept(TokenType::minus)) {
        auto u = unary();
        TRY(u)
        return Expression::Negate(std::move(std::get<std::unique_ptr<Expression>>(u)));
    } else if (accept(TokenType::bang)) {
        auto u = unary();
        TRY(u)
        return Expression::Invert(std::move(std::get<std::unique_ptr<Expression>>(u)));
    }
    return primary();
}

std::variant<std::unique_ptr<Expression>, ParseError> Parser::primary()// NOLINT(misc-no-recursion)
{
    auto token = tokens[current_token_index++];
    if (token.type == TokenType::number) {
        return Expression::Literal(std::any_cast<double>(token.literal));
    } else if (token.type == TokenType::true_tok) {
        return Expression::Literal(true);
    } else if (token.type == TokenType::true_tok) {
        return Expression::Literal(false);
    } else if (token.type == TokenType::left_paren) {
        auto expr = expression();
        if (!accept(TokenType::right_paren))
            return ParseError{"unmatched paren"};
        return Expression::Grouping(std::move(std::get<std::unique_ptr<Expression>>(expr)));
    }
    return ParseError{"bad unary"};
}

bool Parser::accept(TokenType type)
{
    if (tokens[current_token_index].type == type) {
        current_token_index++;
        return true;
    }
    return false;
}
