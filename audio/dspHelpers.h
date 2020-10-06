// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// DSP helper functions

#ifndef _AME_DSP_HELPERS_H_
#define _AME_DSP_HELPERS_H_

#include <limits>

// #include "../test.h"
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
    // TODO add unit test
    // AME_ASSERT_EQ(addModulo2Pi(twoPi<>, pi<>), pi<>);
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
    // TODO add unit test
    AME_ASSERT_EQ(freqToPeriod(1000.0f), 0.001f);

    /** periodToFreq
     * 周期→周波数
     */
    constexpr float periodToFreq(const float period) noexcept
    {
        return 1.0f / period;
    }
    // TODO add unit test
    // AME_ASSERT_EQ(periodToFreq(5.0f), 0.2f);
} // namespace ame

#endif // _AME_DSP_HELPERS_H_
