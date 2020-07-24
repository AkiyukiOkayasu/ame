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
     * add value and modulo 2p
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
    static_assert((addModulo2Pi(twoPi<float>, pi<float>) - pi<float>) <= std::numeric_limits<float>::epsilon(),
                  "error: addModulo2Pi()");

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
    static_assert((freqToPeriod(1000.0f) - 0.001f) <= std::numeric_limits<float>::epsilon(),
                  "error: freqToPeriod()");

    /** periodToFreq
     * 周期→周波数
     */
    constexpr float periodToFreq(const float period) noexcept
    {
        return 1.0f / period;
    }
    static_assert((periodToFreq(5.0f) - 0.2f) <= std::numeric_limits<float>::epsilon(),
                  "error: periodToFreq()");
} // namespace ame

#endif // _AME_DSP_HELPERS_H_
