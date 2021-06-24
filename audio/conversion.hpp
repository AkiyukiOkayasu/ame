/** Format conversion functions.
    @file conversion.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <algorithm>

namespace ame
{
constexpr int32_t Q23_MAX = 8388607;
constexpr int32_t Q23_MIN = -8388608;
constexpr float Q23_ABSMAX = 8388608.0f;

constexpr void float_to_q23 (const float src[], int32_t dest[], const uint32_t blockSize)
{
#if defined(__GNUC__) && (__GNUC__ >= 8)
    #pragma GCC unroll 4
#endif
    for (uint32_t i = 0; i < blockSize; ++i)
    {
        dest[i] = std::clamp (static_cast<int32_t> (src[i] * Q23_MAX), Q23_MIN, Q23_MAX);
    }
}

constexpr void q23_to_float (const int32_t src[], float dest[], const uint32_t blockSize)
{
#if defined(__GNUC__) && (__GNUC__ >= 8)
    #pragma GCC unroll 4
#endif
    for (uint32_t i = 0; i < blockSize; ++i)
    {
        dest[i] = static_cast<float> (src[i]) / Q23_ABSMAX;
    }
}

/** 
    Split channel→Interleave convertion.
    @param source Split channel samples
    @param dest Interleave array
    @param numSamples 
    @param numChannels 
*/
constexpr void interleaveSamples (const float** source, float* dest, const uint_fast32_t numSamples, const uint_fast32_t numChannels)
{
    for (uint_fast32_t chan = 0; chan < numChannels; ++chan)
    {
        auto i = chan;
        auto src = source[chan];

        for (uint_fast32_t j = 0; j < numSamples; ++j)
        {
            dest[i] = src[j];
            i += numChannels;
        }
    }
}

/** 
    Interleave→Split channel convertion.
    @param source Interleave samples
    @param dest Split channel array
    @param numSamples 
    @param numChannels 
*/
inline void deinterleaveSamples (const float* source, float** dest, const uint_fast32_t numSamples, const uint_fast32_t numChannels)
{
    for (uint_fast32_t chan = 0; chan < numChannels; ++chan)
    {
        auto i = chan;
        auto dst = dest[chan];

        for (uint_fast32_t j = 0; j < numSamples; ++j)
        {
            dst[j] = source[i];
            i += numChannels;
        }
    }
}
} // namespace ame