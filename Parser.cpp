#include "Parser.h"

#define TRY(ident)        \
    if (ident.is_error()) \
        return ParserErrorOr(ident.get_error());

ParserErrorOr Parser::parse()
{
    auto maybe_expr = expression();
    TRY(maybe_expr)

    if (current_token_index < tokens.size()) {
        return Error{"Syntax error at token " + std::to_string(current_token_index) + "   " + tokens[current_token_index].string()};
    }

    return maybe_expr;
}

ParserErrorOr Parser::expression()// NOLINT(misc-no-recursion)
{
    return equality();
}

ParserErrorOr Parser::equality()// NOLINT(misc-no-recursion)
{
    auto maybe_expr = comparison();
    TRY(maybe_expr)

    auto expr = std::move(maybe_expr.get_value());

    for (;;) {
        if (accept(TokenType::equals_equals)) {
            maybe_expr = comparison();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::equals, std::move(maybe_expr.get_value()));
        } else if (accept(TokenType::bang_equals)) {
            maybe_expr = comparison();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::not_equals, std::move(maybe_expr.get_value()));
        } else {
            return expr;
        }
    }
}

ParserErrorOr Parser::comparison()// NOLINT(misc-no-recursion)
{
    auto maybe_expr = term();
    TRY(maybe_expr)

    auto expr = std::move(maybe_expr.get_value());

    for (;;) {
        if (accept(TokenType::angle_left)) {
            maybe_expr = term();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::less_than, std::move(maybe_expr.get_value()));
        } else if (accept(TokenType::angle_right)) {
            maybe_expr = term();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::greater_than, std::move(maybe_expr.get_value()));
        } else if (accept(TokenType::angle_left_equals)) {
            maybe_expr = term();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::less_than_equals, std::move(maybe_expr.get_value()));
        } else if (accept(TokenType::angle_right_equals)) {
            maybe_expr = term();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::greater_than_equals, std::move(maybe_expr.get_value()));
        } else {
            return expr;
        }
    }
}

ParserErrorOr Parser::term()// NOLINT(misc-no-recursion)
{
    auto maybe_expr = factor();
    TRY(maybe_expr)

    auto expr = std::move(maybe_expr.get_value());

    for (;;) {
        if (accept(TokenType::plus)) {
            maybe_expr = factor();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::add, std::move(maybe_expr.get_value()));
        } else if (accept(TokenType::minus)) {
            maybe_expr = factor();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::subtract, std::move(maybe_expr.get_value()));
        } else {
            return expr;
        }
    }
}

ParserErrorOr Parser::factor()// NOLINT(misc-no-recursion)
{
    auto maybe_expr = unary();
    TRY(maybe_expr)

    auto expr = std::move(maybe_expr.get_value());

    for (;;) {
        if (accept(TokenType::asterisk)) {
            maybe_expr = unary();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::multiply, std::move(maybe_expr.get_value()));
        } else if (accept(TokenType::slash)) {
            maybe_expr = unary();
            TRY(maybe_expr)

            expr = Expression::Binary(std::move(expr), Operator::divide, std::move(maybe_expr.get_value()));
        } else {
            return expr;
        }
    }
}

ParserErrorOr Parser::unary()// NOLINT(misc-no-recursion)
{
    // TODO: boolean negation
    if (accept(TokenType::minus)) {
        auto u = unary();
        TRY(u)
        return Expression::Negate(std::move(u.get_value()));
    } else if (accept(TokenType::bang)) {
        auto u = unary();
        TRY(u)
        return Expression::Invert(std::move(u.get_value()));
    }
    return primary();
}

ParserErrorOr Parser::primary()// NOLINT(misc-no-recursion)
{
    auto token = tokens[current_token_index++];
    if (token.type == TokenType::number || token.type == TokenType::string) {
        return Expression::Literal(*token.literal);
    } else if (token.type == TokenType::true_tok) {
        return Expression::Literal(true);
    } else if (token.type == TokenType::false_tok) {
        return Expression::Literal(false);
    } else if (token.type == TokenType::left_paren) {
        auto expr = expression();
        TRY(expr)
        if (!accept(TokenType::right_paren))
            return Error{"unmatched paren"};
        return Expression::Grouping(std::move(expr.get_value()));
    }
    return Error{"bad unary"};
}

bool Parser::accept(TokenType type)
{
    if (tokens[current_token_index].type == type) {
        current_token_index++;
        return true;
    }
    return false;
}
