// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// DSP helper functions

#ifndef _AME_DSP_HELPERS_H_
#define _AME_DSP_HELPERS_H_

#include <limits>

#include "../math/constants.h"

namespace ame
{
    /**
     * add value and modulo 2pi
     * return: 0.0f ~ 2pi
     */
    constexpr float addModulo2Pi(float value, const float increment) noexcept
    {
        value += increment;
        while (value > twoPi<float>)
        {
            value -= twoPi<float>;
        }
        return value;
    }    
} // namespace ame

/**
 * Freq  / Period
 * 周波数 /  周期
 */
namespace ame
{
    /** freqToPeriod
     * 周波数→周期
     */
    constexpr float freqToPeriod(const float freq) noexcept
    {
        return 1.0f / freq;
    }

    /** periodToFreq
     * 周期→周波数
     */
    constexpr float periodToFreq(const float period) noexcept
    {
        return 1.0f / period;
    }    
} // namespace ame

#endif // _AME_DSP_HELPERS_H_
