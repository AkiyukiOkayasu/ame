/**
 * @file dspHelpers.hpp
 * @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
 * @brief DSP helper functions
 *
 * @copyright Copyright (c) 2021 Akiyuki Okayasu
 *
 * AME is released under the MIT license.
 */

#pragma once

#include <limits>

#include "../math/constants.hpp"

namespace ame
{
    /**
     * @brief Increment the phase and returns in the range of 0~2pi.
     *
     * @param phase
     * @param increment Amount to add to phase
     * @return 0~2pi
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
