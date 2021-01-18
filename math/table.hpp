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

    // TODO hamming窓とかの実装追加

} // namespace ame