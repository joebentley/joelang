#pragma once
#include <string>
#include <variant>

struct Error {
    std::string message;

    [[nodiscard]] std::string_view string() const { return message; }
};

template<typename T>
struct ErrorOr {
    ErrorOr(Error value) : _value(value) {}//NOLINT(google-explicit-constructor)

    ErrorOr(T value) : _value(std::move(value)) {}//NOLINT(google-explicit-constructor)

    [[nodiscard]] bool is_error() const
    {
        return std::holds_alternative<Error>(_value);
    }

    [[nodiscard]] Error &get_error()
    {
        return std::get<Error>(_value);
    }

    [[nodiscard]] T &get_value()
    {
        return std::get<T>(_value);
    }

private:
    std::variant<Error, T> _value;
};
