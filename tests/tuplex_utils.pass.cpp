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

#include <tuple>

#include "../headers/tuplex.hpp"
#include "../headers/tuplex_utils.hpp"

#include "utils/test_utils.hpp"

TEST(tuplex_utils, get_prvalue)
{
    tuplex::tuple<int, 
                  const int, 
                  volatile int, 
                  const volatile int> tpx{ 1, 2, 3, 4 };

    static_assert(std::is_same_v<int&, decltype(tuplex::get<0>(tpx))>);
    static_assert(std::is_same_v<const int&, decltype(tuplex::get<1>(tpx))>);
    static_assert(std::is_same_v<volatile int&, decltype(tuplex::get<2>(tpx))>);
    static_assert(std::is_same_v<const volatile int&, decltype(tuplex::get<3>(tpx))>);
}

TEST(tuplex_utils, get_cv_ref)
{
    using namespace test_utils;

    for_cv_ref([]<CV_REF type>() -> void
    {
        int a1{};
        float b1{};
        double c1{};

        int a2{};
        float b2{};
        double c2{};

        tuplex::tuple tpx{ as_t<type>(a1), as_t<type>(b1), as_t<type>(c1) };
        std::tuple std_tp{ as_t<type>(a2), as_t<type>(b2), as_t<type>(c2) };

        using indices = std::make_index_sequence<3>;
        
        els_have_same_type(tpx, std_tp, indices{});
        ASSERT_TRUE(els_have_same_value(tpx, std_tp, indices{}));
    });
}

TEST(tuplex_utils, tuple_size)
{
    tuplex::tuple<int, float, std::string, double> tp{ 2, 2.5f, "hello", 5.0 };
    ASSERT_EQ(tuplex::tuple_size_v<decltype(tp)>, 4);
}

TEST(tuplex_utils, forward_as_tuple)
{
    using namespace test_utils;

    for_cv_ref([]<CV_REF type>() -> void
    {
        int a1{};
        float b1{};
        double c1{};

        int a2{};
        float b2{};
        double c2{};

        auto tp{ tuplex::forward_as_tuple(as_t<type>(a1), as_t<type>(b1), as_t<type>(c1)) };
        auto std_tp{ std::forward_as_tuple(as_t<type>(a2), as_t<type>(b2), as_t<type>(c2)) };

        using indices = std::make_index_sequence<3>; 

        els_have_same_type(tp, std_tp, indices{});
        ASSERT_TRUE(els_have_same_value(tp,std_tp, indices{}));
    });
}

TEST(tuplex_utils, make_tuple)
{
    using namespace test_utils;

    for_cv_ref([]<CV_REF type>() -> void
    {
        int a1{};
        float b1{};
        double c1{};

        int a2{};
        float b2{};
        double c2{};

        auto tp{ tuplex::make_tuple(as_t<type>(a1), as_t<type>(b1), as_t<type>(c1)) };
        auto std_tp{ std::make_tuple(as_t<type>(a2), as_t<type>(b2), as_t<type>(c2)) };

        using indices = std::make_index_sequence<3>; 

        els_have_same_type(tp, std_tp, indices{});
        ASSERT_TRUE(els_have_same_value(tp,std_tp, indices{}));
    });  
}

TEST(tuple_utils, cm_counter_cv_ref)
{
    using namespace test_utils;

    for_cv_ref([]<CV_REF type>() -> void
    {
        int a1{};
        float b1{};
        cm_counter c1{};
        
        int a2{};
        float b2{};
        cm_counter c2{};
    
        auto tp{ tuplex::make_tuple(as_t<type>(a1), std::move(b1), c1) };
        auto std_tp{ std::make_tuple(as_t<type>(a2), std::move(b2), c2) };

        using indices = std::make_index_sequence<3>;

        els_have_same_type(tp, std_tp, indices{});
        ASSERT_TRUE(els_have_same_value(tp, std_tp, indices{}));
    });
}

