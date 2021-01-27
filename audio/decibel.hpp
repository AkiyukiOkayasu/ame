/**
    Convert decibels and amplitude.
    
    @file decibel.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    @todo std::pow, std::max, std::log10をより高速かコンパイル時に実行できるものに置き換える
    
    AME is released under the MIT license.
*/

#pragma once

#include <cmath>

namespace ame
{
/** Convert dB to amplitude.
    @param dB decibels value
    @return amplitude [0, 1]
    @note Outputs 0 if the input is less than -100dB.
*/
constexpr float dBToGain (const float dB) noexcept
{
    return dB > -100.0f
               ? std::pow (10.0f, dB * 0.05f)
               : 0.0f; // Outputs 0 if the input is less than -100dB
}

/** Convert amplitude to dB.
    @param gain 
    @return dB decibels value
    @note Outputs Outputs -100dB if the input is less than 0.00001.
*/
constexpr float gainToDecibels (const float gain) noexcept
{
    return gain > 0.00001f
               ? std::log10 (gain) * 20.0f
               : -100.0f; // Minimum output value -100dB
}
} // namespace ame