/**
 * @file q23_float_conversion.hpp
 * @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
 * @brief Convert 32-bit float and Q23
 * @details Complements the missing Q23 in the CMSIS-DSP's Convert 32-bit floating point value @n
 https://www.keil.com/pack/doc/CMSIS/DSP/html/group__float__to__x.html

 *
 * @copyright Copyright (c) 2021 Akiyuki Okayasu
 *
 * AME is released under the MIT license.
 */

#pragma once

#include <algorithm>

namespace ame
{
    constexpr int32_t Q23_MAX  = 8388607;
    constexpr int32_t Q23_MIN  = -8388608;
    constexpr float Q23_ABSMAX = 8388608.0f;

    constexpr void float_to_q23(const float src[], int32_t dest[], const uint32_t blockSize)
    {
#if defined(__GNUC__) && (__GNUC__ >= 8)
#pragma GCC unroll 4
#endif
        for (uint32_t i = 0; i < blockSize; ++i)
        {
            dest[i] = std::clamp(static_cast<int32_t>(src[i] * Q23_MAX), Q23_MIN, Q23_MAX);
        }
    }

    constexpr void q23_to_float(const int32_t src[], float dest[], const uint32_t blockSize)
    {
#if defined(__GNUC__) && (__GNUC__ >= 8)
#pragma GCC unroll 4
#endif
        for (uint32_t i = 0; i < blockSize; ++i)
        {
            dest[i] = static_cast<float>(src[i]) / Q23_ABSMAX;
        }
    }
} // namespace ame