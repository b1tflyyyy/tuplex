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
    } // namespace detail

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

    /* ------------------------------------------------------------------------------------------------ */
    template <typename...>
    struct cat_types;

    template <typename... Ts>
    struct cat_types<tuplex::tuple<Ts...>> : detail::type_identity<tuplex::tuple<Ts...>> {};

    template <typename... Ts1, typename... Ts2, typename... Other>
    struct cat_types<tuplex::tuple<Ts1...>, tuplex::tuple<Ts2...>, Other...> : 
        detail::type_identity<typename cat_types<tuplex::tuple<Ts1..., Ts2...>, Other...>::type> {};

    template <typename... Tps>
    using cat_types_t = typename cat_types<Tps...>::type;

    /* ------------------------------------------------------------------------------------------------ */
    namespace detail
    {
        template <typename AccumulatorTs,
                  typename CurrentIndices,
                  typename NextIndices>
        struct tuple_cat_impl;

        template <typename... AccumulatorTs,
                  std::size_t... CurrentIndices,
                  std::size_t... NextIndices>
        struct tuple_cat_impl<tuplex::tuple<AccumulatorTs...>,
                              std::index_sequence<CurrentIndices...>,
                              std::index_sequence<NextIndices...>>
        {
            template <typename ZeroTuple>
            constexpr auto operator()(tuplex::tuple<AccumulatorTs...>&& accumulator_ts,
                                      ZeroTuple&& zero_tuple) noexcept
            {
                return tuplex::forward_as_tuple(
                    tuplex::get<CurrentIndices>(std::move(accumulator_ts))...,
                    tuplex::get<NextIndices>(std::forward<ZeroTuple>(zero_tuple))...
                );
            } 

            template <typename FirstTuple, typename SecondTuple, typename... Other>
            constexpr auto operator()(tuplex::tuple<AccumulatorTs...>&& accumulator_ts,
                                      FirstTuple&& fst_tuple,
                                      SecondTuple&& snd_tuple,
                                      Other&&... other) noexcept
            {
                auto new_tuple{
                    tuplex::forward_as_tuple(
                        tuplex::get<CurrentIndices>(std::move(accumulator_ts))...,
                        tuplex::get<NextIndices>(std::forward<FirstTuple>(fst_tuple))...
                    )
                };

                using PureSecond = std::remove_cvref_t<SecondTuple>;

                using new_t = decltype(new_tuple);
                
                using current_indices = std::make_index_sequence<tuplex::tuple_size_v<new_t>>;
                using next_indices = std::make_index_sequence<tuplex::tuple_size_v<PureSecond>>;

                using next_t = tuple_cat_impl<new_t,
                                              current_indices,
                                              next_indices>;

                return next_t{}(std::move(new_tuple), 
                                std::forward<SecondTuple>(snd_tuple),
                                std::forward<Other>(other)...);
            }
        };

        template <typename Ret, typename Tp, std::size_t... Indices>
        constexpr Ret tuple_cat_ret_wrapper(Tp&& tp, std::index_sequence<Indices...>) noexcept
        {
            return Ret { 
                tuplex::get<Indices>(std::forward<Tp>(tp))...
            };
        }
    } // namespace detail

    template <typename FirstTuple, typename... Other>
    constexpr auto tuple_cat(FirstTuple&& fst_tuple, Other&&... other) noexcept
    {
        using T1 = std::remove_cvref_t<FirstTuple>;
        using Ret = cat_types_t<T1, std::remove_cvref_t<Other>...>;

        using current_indices = std::make_index_sequence<tuplex::tuple_size_v<T1>>;
        using RetIndicies = std::make_index_sequence<tuplex::tuple_size_v<Ret>>;

        using CatExecutor = detail::tuple_cat_impl<tuplex::tuple<>,
                                                   std::index_sequence<>,
                                                   current_indices>;

        return detail::tuple_cat_ret_wrapper<Ret>(
            CatExecutor{}(
                tuplex::tuple<>{},
                std::forward<FirstTuple>(fst_tuple),
                std::forward<Other>(other)...
            ),
            RetIndicies{}
        );
    }
};
