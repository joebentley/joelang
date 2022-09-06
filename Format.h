#pragma once
#include <cstdio>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

template<typename... Args>
void compiletime_fail(Args...);

#define N_ARGS (sizeof...(Args))

namespace Format {
    namespace Detail {
        template<typename... Args>
        struct CheckedFormatString {
            template<size_t N>
            [[maybe_unused]] consteval CheckedFormatString(const char (&fmt)[N]) : _string{fmt}, _indices{}//NOLINT(google-explicit-constructor)
            {
                size_t num_braces = 0;

                for (auto i = 0; i < N; i++) {
                    if (fmt[i] == '{') {
                        _indices[num_braces] = i;
                        if (fmt[++i] != '}') {
                            compiletime_fail("mismatched brace");
                        } else {
                            num_braces++;
                        }
                    }
                }

                if (N_ARGS != num_braces) {
                    compiletime_fail("not enough slots for number of arguments given");
                }
            }

            std::string_view view()
            {
                return std::string_view(_string);
            }

            std::vector<std::string> strings()
            {
                std::vector<std::string> strings;
                for (auto i = 0; i < N_ARGS + 1; i++) {
                    strings.emplace_back(view().substr(
                            i == 0 ? 0 : _indices[i - 1] + 2,
                            i == N_ARGS ? view().size() - _indices[i - 1] : _indices[i] - (i == 0 ? 0 : _indices[i - 1] + 2)));
                }
                return strings;
            }

        private:
            size_t _indices[N_ARGS];
            const char *_string;
        };
    }// namespace Detail

    template<typename... Args>
    using CheckedFormatString = Detail::CheckedFormatString<std::type_identity_t<Args>...>;

    template<typename... Args>
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"
    std::string fmt(CheckedFormatString<Args...> &&fmtstr, Args... args)
#pragma clang diagnostic pop
    {
        size_t i = 0;

        std::stringstream stream;

        auto strings = fmtstr.strings();

        ([&] {
            stream << strings[i] << args;
            ++i;
        }(),
         ...);

        stream << strings[i];
        return stream.str();
    }

    template<typename... Args>
    [[maybe_unused]] void print(CheckedFormatString<Args...> &&fmtstr, Args... args)
    {
        printf("%s", fmt(std::move(fmtstr), args...).c_str());
    }

    template<typename... Args>
    [[maybe_unused]] void println(CheckedFormatString<Args...> &&fmtstr, Args... args)
    {
        printf("%s\n", fmt(std::move(fmtstr), args...).c_str());
    }
}// namespace Format
