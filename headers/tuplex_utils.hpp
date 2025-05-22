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

#include <cstddef>
#include <type_traits>
#include <concepts>
#include <utility>

#include "tuplex.hpp"

namespace tuplex
{
    namespace detail
    {
        /* ------------------------------------------------------------------------------------------------ */
        template <typename T>
        concept non_cv_ref = std::same_as<T, std::remove_cvref_t<T>>;

        /* ------------------------------------------------------------------------------------------------ */
        template <typename T>
        struct type_identity { using type = T; };

        /* ------------------------------------------------------------------------------------------------ */
        template <typename...>
        struct pop_front;

        template <template <typename...> typename TList, typename T, typename... Ts>
        struct pop_front<TList<T, Ts...>> : type_identity<TList<Ts...>> {};

        template <typename TList>
        using pop_front_t = typename pop_front<TList>::type;

        /* ------------------------------------------------------------------------------------------------ */
        template <typename...>
        struct front;

        template <template <typename...> typename TList, typename T, typename... Ts>
        struct front<TList<T, Ts...>> : type_identity<T> {};

        template <typename TList>
        using front_t = typename front<TList>::type;

        /* ------------------------------------------------------------------------------------------------ */
        template <std::size_t Index, non_cv_ref Tp>
        struct get_impl : get_impl<Index - 1, detail::pop_front_t<Tp>>{ };

        template <non_cv_ref Tp> 
        struct get_impl<0, Tp>
        {
            using data_t = detail::front_t<Tp>;

            constexpr decltype(auto) operator()(Tp& t) noexcept
            {
                return static_cast<data_t&>(static_cast<Tp&>(t).data());
            }

            constexpr decltype(auto) operator()(const Tp& t) const noexcept
            {
                return static_cast<const data_t&>(static_cast<const Tp&>(t).data());
            }

            constexpr decltype(auto) operator()(volatile Tp& t) volatile noexcept
            {
                return static_cast<volatile data_t&>(static_cast<volatile Tp&>(t).data());
            }

            constexpr decltype(auto) operator()(const volatile Tp& t) const volatile noexcept
            {
                return static_cast<const volatile data_t&>(static_cast<const volatile Tp&>(t).data());
            }

            constexpr decltype(auto) operator()(Tp&& t) noexcept
            {
                return static_cast<data_t&&>(static_cast<Tp&&>(t).data());
            }

            constexpr decltype(auto) operator()(const Tp&& t) const noexcept
            {
                return static_cast<const data_t&&>(static_cast<const Tp&&>(t).data());
            }

            constexpr decltype(auto) operator()(volatile Tp&& t) volatile noexcept
            {
                return static_cast<volatile data_t&&>(static_cast<volatile Tp&&>(t).data());
            }

            constexpr decltype(auto) operator()(const volatile Tp&& t) const volatile noexcept
            {
                return static_cast<const volatile data_t&&>(static_cast<const volatile Tp&&>(t).data());
            }
        };
    } // detail

    /* ------------------------------------------------------------------------------------------------ */
    template <typename...>
    struct tuple_size;

    template <template <typename...> typename TList, typename... Ts>
    struct tuple_size<TList<Ts...>>
    {
        static constexpr std::size_t value{ sizeof...(Ts) };
    };

    template <typename TList>
    constexpr std::size_t tuple_size_v{ tuple_size<TList>::value };

    /* ------------------------------------------------------------------------------------------------ */
    template <std::size_t Index, typename Tp>
    constexpr decltype(auto) get(Tp&& t) noexcept
    {
        using T = std::remove_cvref_t<Tp>;
        static_assert(tuple_size_v<T> > Index, "tuple out of range");

        return detail::get_impl<Index, T>{}(std::forward<Tp>(t));
    }

    /* ------------------------------------------------------------------------------------------------ */
    template <typename... Args>
    constexpr tuplex::tuple<Args&&...> forward_as_tuple(Args&&... args) noexcept
    {
        return tuplex::tuple<Args&&...>{ std::forward<Args>(args)... };
    }

    /* ------------------------------------------------------------------------------------------------ */
    template <typename... Args>
    constexpr tuplex::tuple<std::unwrap_ref_decay_t<Args>...> make_tuple(Args&&... args)
    {
        return tuplex::tuple<std::unwrap_ref_decay_t<Args>...>{ std::forward<Args>(args)... };
    }
};
