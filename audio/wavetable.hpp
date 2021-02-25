/** 
    Wave table generator
    @file wavetable.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/
#pragma once

#include "../math/constants.hpp"
#include "../math/specialFunctions.hpp"

#include <array>
#include <numeric>

namespace ame
{
/** Wavetable generator.
    @tparam FloatType float or double
    @tparam N array size
    @tparam Function function to generate wavetable
    @note Functionの入力は0~1範囲の配列です
    @todo FloatTypeいらないかも。floatだけでいい？
    @see make_sineTable()
*/
template <typename FloatType, size_t N, class Function>
constexpr auto make_waveTable (Function func)
{
    std::array<FloatType, N> ar;
    std::iota (ar.begin(), ar.end(), 0);
    std::for_each (ar.begin(), ar.end(), [] (auto& x) { x = x / (N - 1); }); // ar: 0~1が順に並んだ配列
    std::for_each (ar.begin(), ar.end(), func);
    return ar;
}

/** Sine wave wavetable generator.
    @tparam N array size
    @return constexpr std::array<float, N> sine wave table
    @todo 初期位相いじれるようにする？
*/
template <size_t N>
constexpr auto make_sineTable()
{
    auto f = [] (auto& x) { x = ame::sinf (x * ame::twoPi); };
    return ame::make_waveTable<float, N> (f);
}
} // namespace ame