/**
 * @file table.hpp
 * @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
 * @brief
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include "constants.hpp"
#include "specialFunctions.hpp"
#include <array>

namespace ame
{
    /**
     * @brief Compile time sine wave table generator
     *
     * @tparam N Num samples
     * @return constexpr std::array<float, N> sine wave table
     */
    template<size_t N> constexpr std::array<float, N> make_sineTable()
    {
        std::array<float, N> a{};
        for (size_t i = 0; i < N; ++i)
        {
            float phase = static_cast<float>(i) / static_cast<float>(N) * twoPi;
            a[i]        = ame::sinf(phase);
        }
        return a;
    }

    // TODO hamming窓や矩形波などの実装追加

} // namespace ame

/*
より汎用的な実装
https://www.it-swarm-ja.tech/ja/c++/%E6%9C%80%E6%96%B0%E3%81%AEc-%EF%BC%9Aconstexpr%E3%83%86%E3%83%BC%E3%83%96%E3%83%AB%E3%81%AE%E5%88%9D%E6%9C%9F%E5%8C%96/835602258/

template<class T, size_t N, class F>
constexpr auto make_table(F func, T first)
{
    std::array<T, N> a {first};
    for (size_t i = 1; i < N; ++i)
    {
        a[i] = func(a[i - 1]);
    }
    return a;
}
*/