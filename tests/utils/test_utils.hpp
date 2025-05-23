// MIT License
// 
// Copyright (c) 2025 Mr. Myxa
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <iostream>
#include <format>
#include <cstddef>
#include <utility>
#include <concepts>
#include <functional>
#include <cstdint>

namespace test_utils
{
    class cm_counter // copy,move counter
    {
    public: 
        constexpr cm_counter() = default;

        cm_counter(const cm_counter& rhs) :
            copies{ rhs.copies + 1 }, moves{ rhs.moves }
        { }

        cm_counter(cm_counter&& rhs) noexcept :
            copies{ rhs.copies }, moves{ rhs.moves + 1 }
        { }

        bool equals(const cm_counter& rhs) const noexcept
        {
            return copies == rhs.copies && moves == rhs.moves;
        }

        friend bool operator==(const cm_counter& lhs, const cm_counter& rhs) noexcept
        {
            return lhs.equals(rhs);
        }

        friend std::ostream& operator<<(std::ostream& os, const cm_counter& c)
        {
            os << std::format("[cm_counter copies: {}, moves: {}", c.copies, c.moves);
            return os;
        };

    private:
        std::size_t copies{};
        std::size_t moves{};
    };

    enum class CV_REF
    {
        PLAIN_REF,
        CONST_REF,
        VOLATILE_REF,
        CONST_VOLATILE_REF,
        PLAIN_REF_REF,
        CONST_REF_REF,
        VOLATILE_REF_REF,
        CONST_VOLATILE_REF_REF
    };

    template <CV_REF cv_ref, typename T>
    constexpr decltype(auto) as_t(T& value) noexcept
    {
        if constexpr (cv_ref == CV_REF::PLAIN_REF)
        {
            return static_cast<T&>(value);
        }
        else if constexpr (cv_ref == CV_REF::CONST_REF)
        {
            return static_cast<const T&>(value);
        }
        else if constexpr (cv_ref == CV_REF::VOLATILE_REF)
        {
            return static_cast<volatile T&>(value);
        }
        else if constexpr (cv_ref == CV_REF::CONST_VOLATILE_REF)
        {
            return static_cast<const volatile T&>(value);
        }
        else if constexpr (cv_ref == CV_REF::PLAIN_REF_REF)
        {
            return static_cast<T&&>(value);
        }
        else if constexpr (cv_ref == CV_REF::CONST_REF_REF)
        {
            return static_cast<const T&&>(value);
        }
        else if constexpr (cv_ref == CV_REF::VOLATILE_REF_REF)
        {
            return static_cast<volatile T&&>(value);
        }
        else if constexpr (cv_ref == CV_REF::CONST_VOLATILE_REF_REF)
        {
            return static_cast<const volatile T&&>(value);
        }
    }

    template <CV_REF... Sequence>
    struct cv_refs_holder {};

    template <CV_REF... Seq, typename Func>
    constexpr void for_cv_ref_impl(Func&& func, cv_refs_holder<Seq...> = {}) noexcept
    {
        ((std::forward<Func>(func).template operator()<Seq>()), ...);
    }

    template <typename Func>
    constexpr void for_cv_ref(Func&& func) noexcept
    {
        for_cv_ref_impl<CV_REF::PLAIN_REF,
                        CV_REF::CONST_REF,
                        CV_REF::VOLATILE_REF,
                        CV_REF::CONST_VOLATILE_REF,
                        CV_REF::PLAIN_REF_REF,
                        CV_REF::CONST_REF_REF,
                        CV_REF::VOLATILE_REF_REF,
                        CV_REF::CONST_VOLATILE_REF_REF>(std::forward<Func>(func));
    }

    template <typename Tuple1, typename Tuple2, std::size_t... Indices>
    constexpr void els_have_same_type(Tuple1&& tp1, Tuple2&& tp2, std::index_sequence<Indices...>) noexcept
    {
        static_assert(
            ((std::is_same_v<decltype(get<Indices>(std::forward<Tuple1>(tp1))), 
                             decltype(get<Indices>(std::forward<Tuple2>(tp2)))>) && ...)
        );
    }

    template <typename Tuple1, typename Tuple2, std::size_t... Indices>
    constexpr bool els_have_same_value(Tuple1&& tp1, Tuple2&& tp2, std::index_sequence<Indices...>) noexcept
    {
        return ((get<Indices>(std::forward<Tuple1>(tp1)) == 
                 get<Indices>(std::forward<Tuple2>(tp2))) && ...);
    }
}