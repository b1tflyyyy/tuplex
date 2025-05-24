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

TEST(tuple_utils, cat_types)
{
    using T1 = tuplex::tuple<int, float, std::string>;
    using T2 = tuplex::tuple<int&&, float&, std::string>;
    using T3 = tuplex::tuple<int>;

    static_assert(std::is_same_v<tuplex::cat_types_t<T1, T2, T3>, 
                                 tuplex::tuple<int, float, std::string, int&&, float&, std::string, int>>);
}

TEST(tuple_utils, tuple_cat_binary)
{
    auto t1{ tuplex::make_tuple(1, 3.4f, "hello world") };
    auto t2{ tuplex::make_tuple(1, 2, 3) };

    auto res{ tuplex::tuple_cat(t1, t2) };

    using Res = decltype(res);
    static_assert(std::is_same_v<Res, tuplex::tuple<int, float, const char*, int, int, int>>);
}

TEST(tuple_utils, tuple_cat_ternary)
{
    int a{ 1 };
    float b{ 2.3f };
    std::string c{ "hello world" };

    tuplex::tuple<int, float, std::string> t1{ 1, 3.4f, "hello world" };
    tuplex::tuple<int, int, int> t2{ 1, 2, 3 };
    tuplex::tuple<int&&, float&, std::string> t3{ std::move(a), b, c };

    auto res{ tuplex::tuple_cat(t1, t2, std::move(t3)) };
    
    using Res = decltype(res);
    static_assert(std::is_same_v<Res, tuplex::tuple<int, float, std::string, int, int, int, int&&, float&, std::string>>);
}

TEST(tuple_utils, tuple_cat_vs_std)
{
    using namespace test_utils;

    for_cv_ref([]<CV_REF type>() -> void
    {
        int a1{};
        cm_counter cm1{};

        int a2{};
        cm_counter cm2{};

        auto t1{ tuplex::make_tuple(as_t<type>(a1), cm1) };
        auto t2{ tuplex::make_tuple(1, 2, 3) };
        
        auto s1{ std::make_tuple(as_t<type>(a2), cm2) };
        auto s2{ std::make_tuple(1, 2, 3) };

        auto res1{ tuplex::tuple_cat(t1, t2) };
        auto res2{ std::tuple_cat(s1, s2) };

        using indices = std::make_index_sequence<5>;

        els_have_same_type(res1, res2, indices{});
        ASSERT_TRUE(els_have_same_value(res1, res2, indices{}));
    });
}