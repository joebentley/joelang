#include "Lexer.h"
#include "Parser.h"
#include <JK/Format.h>

int main()
{
    //    Lexer lexer("(13.4 + 3) * 4 + 4 / 4");
    //    Lexer lexer("true == false < 4");
    //    Lexer lexer(R"("Hello" + " world!")");
    //    Lexer lexer("10 % 3 ^ 3");
    Lexer lexer("false || (5 > 3)");
    auto maybe_tokens = lexer.lex();

    if (maybe_tokens.is_error()) {
        Format::println("Lexer Error: {}", maybe_tokens.get_error().string());
        return -1;
    }

    auto tokens = maybe_tokens.get_value();

    for (const auto &token: tokens) {
        Format::println("{}", token.string());
    }

    auto expression = Parser(tokens).parse();

    if (expression.is_error()) {
        Format::println("Parsing Error: {}", expression.get_error().string());
        return -1;
    }

    Format::println("{}", (*expression)->string());

    auto evaled = (*expression)->evaluate();

    if (evaled.is_error()) {
        Format::println("Evaluation Error: {}", evaled.get_error().string());
        return -1;
    }

    Format::println("{}", evaled->string());

    return 0;
}
