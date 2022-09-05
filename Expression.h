#pragma once
#include "Error.h"
#include <memory>
#include <optional>
#include <string>

enum class Operator {
    add,
    subtract,
    multiply,
    divide,
    equals,
    not_equals,
    greater_than,
    greater_than_equals,
    less_than,
    less_than_equals,
    negate,
    invert
};

std::string op_to_string(Operator op);

enum class ExpressionType {
    binary,
    literal,
    grouping,
    unary
};

struct Expression;
using EvaluationErrorOr = ErrorOr<Expression>;

struct Expression {
    std::optional<ExpressionType> type;
    std::optional<Operator> op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

    std::variant<double, bool> value;

    Expression(std::optional<ExpressionType> type,
               std::optional<Operator> op,
               std::unique_ptr<Expression> left,
               std::unique_ptr<Expression> right) : type(type), op(op), left(std::move(left)), right(std::move(right)) {}

    explicit Expression(std::variant<double, bool> value) : type(ExpressionType::literal), value(value) {}

    Expression(std::unique_ptr<Expression> left, Operator op, std::unique_ptr<Expression> right) : type(ExpressionType::binary), left(std::move(left)), op(op), right(std::move(right)) {}

    explicit Expression(std::unique_ptr<Expression> expr) : type(ExpressionType::grouping), left(std::move(expr)) {}

    static std::unique_ptr<Expression> Binary(std::unique_ptr<Expression> left, Operator op, std::unique_ptr<Expression> right)
    {
        return std::make_unique<Expression>(std::move(left), op, std::move(right));
    }

    static std::unique_ptr<Expression> Literal(std::variant<double, bool> value)
    {
        return std::make_unique<Expression>(value);
    }

    static std::unique_ptr<Expression> Grouping(std::unique_ptr<Expression> expression)
    {
        return std::make_unique<Expression>(std::move(expression));
    }

    static std::unique_ptr<Expression> Negate(std::unique_ptr<Expression> expression)
    {
        return std::make_unique<Expression>(ExpressionType::unary, Operator::negate, std::move(expression), nullptr);
    }

    static std::unique_ptr<Expression> Invert(std::unique_ptr<Expression> expression)
    {
        return std::make_unique<Expression>(ExpressionType::unary, Operator::invert, std::move(expression), nullptr);
    }

    [[nodiscard]] std::string string() const;

    EvaluationErrorOr evaluate();
};
