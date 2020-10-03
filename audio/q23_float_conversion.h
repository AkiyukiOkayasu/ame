// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// Convert 32-bit float and Q23
// Complements the missing Q23 in the CMSIS-DSP's Convert 32-bit floating point value ( https://www.keil.com/pack/doc/CMSIS/DSP/html/group__float__to__x.html )

#ifndef _AME_Q23_FLOAT_CONVERSION_H_
#define _AME_Q23_FLOAT_CONVERSION_H_

#include <algorithm>

namespace ame
{
    constexpr int32_t Q23_MAX = 8388607;
    constexpr int32_t Q23_MIN = -8388608;

    constexpr void float_to_q23(const float src[], int32_t dest[], const uint32_t blockSize)
    {
#if defined(__GNUC__) && (__GNUC__ >= 8)
#pragma GCC unroll 4
#endif
        for (uint32_t i = 0; i < blockSize; ++i)
        {
            dest[i] = std::clamp(static_cast<int32_t>(src[i] * Q23_MAX),
                                 Q23_MIN,
                                 Q23_MAX);
        }
    }

    constexpr void q23_to_float(const int32_t src[], float dest[], const uint32_t blockSize)
    {
#if defined(__GNUC__) && (__GNUC__ >= 8)
#pragma GCC unroll 4
#endif
        for (uint32_t i = 0; i < blockSize; ++i)
        {
            dest[i] = src[i] / 8388608.0f;
        }
    }
} // namespace ame

#endif // Q23_FLOAT_CONVERSION