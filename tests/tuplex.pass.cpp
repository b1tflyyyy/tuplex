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

#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "../headers/tuplex.hpp"

#include "utils/test_utils.hpp"

TEST(tuplex, ctor_without_cv)
{
    test_utils::cm_counter cm1{};
    test_utils::cm_counter cm2{};

    tuplex::tuple<int, float, std::string, test_utils::cm_counter> tp{ 3, 4.5f, "hello", cm1 };
    std::tuple<int, float, std::string, test_utils::cm_counter> std_tp{ 3, 4.5f, "hello", cm2 };

    std::cout << "tuplex cm: " << cm1 << '\n';
    std::cout << "std::tuple cm: " << cm2 << '\n';

    ASSERT_EQ(cm1, cm2);
}

TEST(tuplex, ctor_with_cv)
{
    test_utils::cm_counter cm1{};
    test_utils::cm_counter cm2{};

    tuplex::tuple<const volatile int, volatile float, const std::string, volatile test_utils::cm_counter> tp{ 3, 4.5f, "hello", cm1 };
    std::tuple<const volatile int, volatile float, const std::string, volatile test_utils::cm_counter> std_tp{ 3, 4.5f, "hello", cm2 };

    std::cout << "tuplex cm: " << cm1 << '\n';
    std::cout << "std::tuple cm: " << cm2 << '\n';

    ASSERT_EQ(cm1, cm2);
}