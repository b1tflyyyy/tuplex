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
}