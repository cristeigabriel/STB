/**
 * @file stb.hh
 * @author Cristei Gabriel-Marian (cristei.g772@gmail.com)
 * @brief Compile-time String To Bytes (STB)
 * @version 0.1
 * @date 2021-09-06
 * 
 */

#ifndef STB_DEFINED
#define STB_DEFINED

#include <cstddef>
#include <array>

namespace stb {
namespace detail {
    // detail methods assume null terminator.

    template<size_t N>
    constexpr auto find_first_of_start(const std::array<char, N>& data, size_t start, char ch) {
        size_t idx = start;
        while (data[idx] != ch && idx < N)
            ++idx;

        return idx;
    }

    template<size_t N>
    constexpr auto find_first_not_of_start(const std::array<char, N>& data, size_t start, char ch) {
        if (start < N && data[start] != ch)
            return start;

        size_t idx = start;
        while (data[idx] == ch && idx < N)
            ++idx;

        return idx;
    }

    template<size_t N>
    constexpr auto find_last_of(const std::array<char, N>& data, char ch) {
        size_t idx = data.size() - 2;
        while (data[idx] != ch && idx >= 0)
            --idx;

        return idx;
    }

    template<size_t N>
    constexpr auto find_last_not_of(const std::array<char, N>& data, char ch) {
        size_t idx = data.size() - 2;
        while (data[idx] == ch && idx >= 0)
            --idx;

        return idx;
    }

    constexpr auto char_to_hex(char ch) {
        if (ch >= '0' && ch <= '9')
            return ch - '0';

        if (ch >= 'A' && ch <= 'F')
            return ch - 'A' + 10;

        return ch - 'a' + 10;
    }

    template<typename T, T F = 16>
    constexpr T concat_hex(T lhs, T rhs) {
        return F * lhs + rhs;
    }
}  // namespace detail

template<auto R>
struct compiletime_value {
    using type                  = decltype(R);
    constexpr static type value = R;
};

//	string wrapper
//	potential todo: make interface class that includes detail utilities at shorthand?
template<size_t N>
struct compiletime_string_wrapper {
    constexpr static auto length = N;

    constexpr compiletime_string_wrapper(const char* str) {
        for (auto i = 0; i != N; ++i)
            data[i] = str[i];
    }

    constexpr auto operator()() const {
        return data;
    }

    std::array<char, length + 1> data = {};
};

template<size_t N>
compiletime_string_wrapper(const char (&)[N]) -> compiletime_string_wrapper<N - 1>;

template<char delimiter, char mask, int masked>
struct basic_compiletime_string_to_byte_array_data {
    template<compiletime_string_wrapper str>
    struct getter {
      private:
        struct parse {
            struct result {
                size_t delimiter_count;
                size_t start;
                size_t next;
                size_t end;
            };

            constexpr static auto get() {
                size_t count = 1;

                constexpr size_t start = detail::find_first_not_of_start(str(), 0, delimiter);
                constexpr size_t next  = detail::find_first_of_start(str(), start, delimiter);
                constexpr size_t end   = detail::find_last_not_of(str(), delimiter);

                bool previous_delimiter = false;
                for (auto i = next; i < end; ++i) {
                    if (str()[i] == delimiter) {
                        if (!previous_delimiter)
                            ++count;

                        previous_delimiter = true;
                    } else
                        previous_delimiter = false;
                }

                return result {
                    count,
                    start,
                    next,
                    end};
            }
        };

        constexpr static auto get() {
            constexpr auto data  = parse::get();
            constexpr auto count = data.delimiter_count;
            constexpr auto start = data.start;
            constexpr auto next  = data.next;
            constexpr auto end   = data.end;

            std::array<int, count> result   = {};
            std::array<size_t, count> skips = {};
            size_t skipped                  = 0;
            size_t traversed                = start;

            bool previous_skip = false;
            for (auto i = start; i < end; ++i) {
                if (str()[i] == delimiter) {
                    if (!previous_skip)
                        skips[skipped++] = traversed;

                    previous_skip = true;
                } else
                    previous_skip = false;

                ++traversed;
            }

            result[0] = str()[start] == mask ? masked : detail::concat_hex(detail::char_to_hex(str()[start]), detail::char_to_hex(str()[start + 1]));

            size_t conversions = 1;
            for (auto i = next; i < end; ++i) {
                for (auto entry : skips) {
                    if (entry == i && entry < end) {
                        size_t idx            = detail::find_first_not_of_start(str(), i + 1, delimiter);
                        bool one_char         = str()[idx + 1] == delimiter;
                        result[conversions++] = str()[idx] == mask ? masked : (one_char ? detail::char_to_hex(str()[idx]) : detail::concat_hex(detail::char_to_hex(str()[idx]), detail::char_to_hex(str()[idx + 1])));
                    }
                }
            }

            return result;
        }

      public:
        constexpr static auto value = compiletime_value<get()>::value;
    };
};

using compiletime_string_to_byte_array_data = basic_compiletime_string_to_byte_array_data<' ', '?', -1>;
}  // namespace stb

#endif