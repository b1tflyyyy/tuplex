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

#include <utility>

namespace tuplex 
{
    template <typename...>
    class tuple
    {
    public:
        constexpr tuple() = default;
        constexpr ~tuple() noexcept = default;
    };

    template <typename T, typename... Ts>
    class tuple<T, Ts...> : public tuple<Ts...>
    {
    public:
        template <typename U, typename... Us>
        constexpr explicit tuple(U&& u, Us&&... us) :
            tuple<Ts...>{ std::forward<Us>(us)... }, mData{ std::forward<U>(u) }
        { }

        constexpr T& data() noexcept
        {
            return mData;
        }

        constexpr const T& data() const noexcept
        {
            return mData;
        }

        constexpr volatile T& data() volatile noexcept
        {
            return mData;
        }

        constexpr const volatile T& data() const volatile noexcept
        {
            return mData;
        }

    private:
        T mData;
    };

    template <typename U, typename... Us>
    tuple(U&&, Us&&...) -> tuple<U, Us...>;
}