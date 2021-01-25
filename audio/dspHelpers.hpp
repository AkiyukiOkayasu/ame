/** 
    DSP helper functions

    @file dspHelpers.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/
#pragma once

#include "../math/constants.hpp"

#include <limits>

namespace ame
{
// TODO mapping.hppへ移動？
/** Linear interpolation between a and b.
    @note Function as same as std::lerp() implemented in C++20.
    @param a Start point
    @param b Target point
    @param t It expects a value of 0~1 to be entered, but specifications exceeding 1 are allowed.
    @return linear interpolated value
*/
constexpr float lerp(float a, float b, float t) noexcept { return a + t * (b - a); }

// TODO phase.hppへ移動？
/** Increment the phase and returns in the range of 0~2pi.
    @param phase
    @param increment Amount to add to phase
    @return 0~2pi
*/
constexpr float addModulo2Pi(float phase, const float increment) noexcept
{
    phase += increment;
    while (phase > twoPi)
    {
        phase -= twoPi;
    }
    return phase;
}

// mapping.hppへ移動？
/** Map values to an output range.
    @param sourceValue input value
    @param sourceRangeMin input range min
    @param sourceRangeMax input range Max
    @param targetRangeMin output range min
    @param targetRangeMax output range Max
    @return scaled value
*/
constexpr float scale(const float sourceValue,
                      const float sourceRangeMin,
                      const float sourceRangeMax,
                      const float targetRangeMin,
                      const float targetRangeMax)
{
    return targetRangeMin
           + ((targetRangeMax - targetRangeMin) * (sourceValue - sourceRangeMin)) / (sourceRangeMax - sourceRangeMin);
}

// Frequency.hppへ移動？
/** Convert frequency to period.
    @param freq frequency in Hz
    @return period
*/
constexpr float freqToPeriod(const float freq) noexcept { return 1.0f / freq; }

// Frequency.hppへ移動？
/** Convert period to frequency.
    @param period period
    @return frequency
*/
constexpr float periodToFreq(const float period) noexcept { return 1.0f / period; }
}// namespace ame
