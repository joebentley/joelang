#include "Lexer.h"
#include "Parser.h"
#include <iostream>

int main()
{
    Lexer lexer("!((13.4 + 3) * 4 + 4 > 70)");
    //    Lexer lexer("true != !true");
    auto maybe_tokens = lexer.lex();

    if (maybe_tokens.is_error()) {
        std::cerr << "Lexer Error: " << maybe_tokens.get_error().string() << std::endl;
        return -1;
    }

    auto tokens = maybe_tokens.get_value();

    for (const auto &token: tokens) {
        std::cout << token.string() << std::endl;
    }

    auto expression = Parser(tokens).parse();

    if (expression.is_error()) {
        std::cerr << "Parsing Error: " << expression.get_error().string() << std::endl;
        return -1;
    }

    auto expr = std::move(expression.get_value());

    std::cout << expr->string() << std::endl;

    auto evaled = expr->evaluate();

    if (!evaled.has_value()) {
        std::cerr << "Evaluation error" << std::endl;
        return -1;
    }

    std::cout << evaled->string() << std::endl;

    return 0;
}
