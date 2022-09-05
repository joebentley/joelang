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
        std::string op_string = op_to_string(*op);
        return left->string() + " " + op_string + " " + right->string();
    }
    return "";
}

std::string op_to_string(Operator op)
{
    switch (op) {
        case Operator::add:
            return "+";
        case Operator::subtract:
            return "-";
        case Operator::multiply:
            return "*";
        case Operator::divide:
            return "/";
        case Operator::equals:
            return "==";
        case Operator::not_equals:
            return "!=";
        case Operator::greater_than:
            return ">";
        case Operator::greater_than_equals:
            return ">=";
        case Operator::less_than:
            return "<";
        case Operator::less_than_equals:
            return "<=";
        case Operator::negate:
            return "-";
        case Operator::invert:
            return "!";
        default:
            assert(false);
    }
}

EvaluationErrorOr Expression::evaluate()// NOLINT(misc-no-recursion)
{
    if (!type.has_value())
        return Error{"Expression has no type"};

    if (*type == ExpressionType::binary) {
        if (!op.has_value())
            return Error{"Binary expression missing operator"};

        auto lhs = left->evaluate();
        if (lhs.is_error())
            return lhs.get_error();
        auto rhs = right->evaluate();
        if (rhs.is_error())
            return rhs.get_error();

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
                    return Error{"Invalid operator for two doubles: " + lhs->string() + " " + op_to_string(*op) + " " + rhs->string()};
            }
        } else if (std::holds_alternative<bool>(lhs->value) && std::holds_alternative<bool>(rhs->value)) {
            auto lhs_bool = std::get<bool>(lhs->value);
            auto rhs_bool = std::get<bool>(rhs->value);
            switch (*op) {
                case Operator::equals:
                    return Expression(lhs_bool == rhs_bool);
                case Operator::not_equals:
                    return Expression(lhs_bool != rhs_bool);
                default:
                    return Error{"Invalid operator for two bools: " + lhs->string() + " " + op_to_string(*op) + " " + rhs->string()};
            }
        } else {
            return Error{"Mismatched operand types in expression: " + lhs->string() + " " + op_to_string(*op) + " " + rhs->string()};
        }
    } else if (*type == ExpressionType::literal) {
        return Expression(value);
    } else if (*type == ExpressionType::unary) {
        if (left == nullptr)
            return Error{"Missing operand for unary expression: " + string()};
        auto operand = left->evaluate();
        if (operand.is_error())
            return operand.get_error();

        if (std::holds_alternative<double>(operand->value)) {
            if (*op == Operator::negate) {
                return Expression(-std::get<double>(operand->value));
            } else {
                return Error{"Invalid unary operator for double: " + string()};
            }
        } else if (std::holds_alternative<bool>(operand->value)) {
            if (*op == Operator::invert) {
                return Expression(!std::get<bool>(operand->value));
            } else {
                return Error{"Invalid unary operator for bool: " + string()};
            }
        } else {
            assert(false);
        }
    } else if (*type == ExpressionType::grouping) {
        if (left == nullptr)
            return Error{"Missing operand for grouping expression: " + string()};
        auto operand = left->evaluate();
        if (operand.is_error())
            return operand.get_error();

        return Expression(operand->value);
    }

    assert(false);
    //    return Error{"Unknown expression type??"};
}
