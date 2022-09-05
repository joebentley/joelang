#include "Expression.h"
#include <cassert>

std::string Expression::string() const// NOLINT(misc-no-recursion)
{
    if (type == ExpressionType::literal) {
        if (std::holds_alternative<double>(value))
            return std::to_string(std::get<double>(value));
        else if (std::holds_alternative<bool>(value)) {
            if (std::get<bool>(value)) {
                return "true";
            } else {
                return "false";
            }
        }
    } else if (type == ExpressionType::unary) {
        if (*op == Operator::negate) {
            return "-" + left->string();
        } else if (*op == Operator::invert) {
            return "!" + left->string();
        }
    } else if (type == ExpressionType::grouping) {
        return "(" + left->string() + ")";
    } else if (type == ExpressionType::binary) {
        std::string op_string;
        switch (*op) {
            case Operator::add:
                op_string = "+";
                break;
            case Operator::subtract:
                op_string = "-";
                break;
            case Operator::multiply:
                op_string = "*";
                break;
            case Operator::divide:
                op_string = "/";
                break;
            case Operator::equals:
                op_string = "==";
                break;
            case Operator::not_equals:
                op_string = "!=";
                break;
            case Operator::greater_than:
                op_string = ">";
                break;
            case Operator::greater_than_equals:
                op_string = ">=";
                break;
            case Operator::less_than:
                op_string = "<";
                break;
            case Operator::less_than_equals:
                op_string = "<=";
                break;
            default:
                assert(false);
        }

        return left->string() + " " + op_string + " " + right->string();
    }
    return "";
}

std::optional<Expression> Expression::evaluate()// NOLINT(misc-no-recursion)
{
    if (!type.has_value())
        return {};

    if (*type == ExpressionType::binary) {
        if (!op.has_value())
            return {};

        auto lhs = left->evaluate();
        if (!lhs.has_value())
            return {};
        auto rhs = right->evaluate();
        if (!rhs.has_value())
            return {};

        // TODO: error checking
        if (std::holds_alternative<double>(lhs->value) && std::holds_alternative<double>(rhs->value)) {
            auto lhs_double = std::get<double>(lhs->value);
            auto rhs_double = std::get<double>(rhs->value);
            switch (*op) {
                case Operator::add:
                    return Expression(lhs_double + rhs_double);
                case Operator::subtract:
                    return Expression(lhs_double - rhs_double);
                case Operator::multiply:
                    return Expression(lhs_double * rhs_double);
                case Operator::divide:
                    return Expression(lhs_double / rhs_double);
                case Operator::less_than:
                    return Expression(lhs_double < rhs_double);
                case Operator::greater_than:
                    return Expression(lhs_double > rhs_double);
                case Operator::less_than_equals:
                    return Expression(lhs_double <= rhs_double);
                case Operator::greater_than_equals:
                    return Expression(lhs_double >= rhs_double);
                case Operator::equals:
                    return Expression(lhs_double == rhs_double);
                case Operator::not_equals:
                    return Expression(lhs_double != rhs_double);
                default:
                    return {};
            }
        }

        if (std::holds_alternative<bool>(lhs->value) && std::holds_alternative<bool>(rhs->value)) {
            auto lhs_bool = std::get<bool>(lhs->value);
            auto rhs_bool = std::get<bool>(rhs->value);
            switch (*op) {
                case Operator::equals:
                    return Expression(lhs_bool == rhs_bool);
                case Operator::not_equals:
                    return Expression(lhs_bool != rhs_bool);
                default:
                    return {};
            }
        }
    } else if (*type == ExpressionType::literal) {
        return Expression(value);
    } else if (*type == ExpressionType::unary) {
        if (left == nullptr)
            return {};
        auto operand = left->evaluate();
        if (!operand.has_value())
            return {};

        // TODO: error checking
        if (std::holds_alternative<double>(operand->value) && *op == Operator::negate)
            return Expression(-std::get<double>(operand->value));

        if (std::holds_alternative<bool>(operand->value) && *op == Operator::invert)
            return Expression(!std::get<bool>(operand->value));
    } else if (*type == ExpressionType::grouping) {
        if (left == nullptr)
            return {};
        auto operand = left->evaluate();
        if (!operand.has_value())
            return {};

        return Expression(operand->value);
    }

    return {};
}
