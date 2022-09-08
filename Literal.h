#pragma once
#include <string>
#include <variant>

using Literal = std::variant<double, bool, std::string>;
