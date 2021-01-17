// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// DSP helper functions

#pragma once

#include <limits>

#include "../math/constants.hpp"

namespace ame
{
    /**
     * @brief add value and modulo 2pi
     * @return 0.0f ~ 2pi
     */
    constexpr float addModulo2Pi(float value, const float increment) noexcept
    {
        value += increment;
        while (value > twoPi)
        {
            value -= twoPi;
        }
        return value;
    }

    /*

    */

    /**
     * @brief Map values to an output range. @n Same function as the Cycling'74 Max [scale] object.
     *
     * @param sourceValue input value
     * @param sourceRangeMin input range min
     * @param sourceRangeMax input range Max
     * @param targetRangeMin output range min
     * @param targetRangeMax output range Max
     * @return scaled value
     */
    constexpr float scale(const float sourceValue, const float sourceRangeMin, const float sourceRangeMax,
                          const float targetRangeMin, const float targetRangeMax)
    {
        return targetRangeMin +
               ((targetRangeMax - targetRangeMin) * (sourceValue - sourceRangeMin)) / (sourceRangeMax - sourceRangeMin);
    }

    /**
     * @brief Convert frequency to period.
     * @param freq frequency in Hz
     * @return period
     */
    constexpr float freqToPeriod(const float freq) noexcept
    {
        return 1.0f / freq;
    }

    /** periodToFreq
     * 周期→周波数
     */

    /**
     * @brief Convert period to frequency.
     * @param period period
     * @return frequency
     */
    constexpr float periodToFreq(const float period) noexcept
    {
        return 1.0f / period;
    }
} // namespace ame
