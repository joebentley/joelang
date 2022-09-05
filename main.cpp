#include "Lexer.h"
#include "Parser.h"
#include <iostream>

int main()
{
    Lexer lexer("!((13.4 + 3) * 4 + 4 > 70)");
    //    Lexer lexer("true != !true");
    auto maybe_tokens = lexer.lex();

    if (std::holds_alternative<LexError>(maybe_tokens)) {
        std::cerr << std::get<LexError>(maybe_tokens).string() << std::endl;
        return -1;
    }

    auto tokens = std::get<std::vector<Token>>(maybe_tokens);

    for (const auto &token: tokens) {
        std::cout << token.string() << std::endl;
    }

    auto expression = Parser(tokens).parse();

    if (std::holds_alternative<ParseError>(expression)) {
        std::cerr << "Parsing Error: " << std::get<ParseError>(expression).string() << std::endl;
        return -1;
    }

    auto expr = std::move(std::get<std::unique_ptr<Expression>>(expression));

    std::cout << expr->string() << std::endl;

    auto evaled = expr->evaluate();

    if (!evaled.has_value()) {
        std::cerr << "Evaluation error" << std::endl;
        return -1;
    }

    std::cout << evaled->string() << std::endl;

    return 0;
}
