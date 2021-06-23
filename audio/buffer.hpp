/** 
    Audio buffer.
    @file buffer.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <array>

namespace ame
{
// AudioBuffer
// 二次元配列版std::arrayの型エイリアス
// 使用例：AudioBuffer<int16_t, 2, 512> 2ch, 512samples
template <typename T, size_t channels, size_t samples>
using AudioBuffer = std::array<std::array<T, samples>, channels>;

// AudioMonoBuffer
// std::arrayの型エイリアス
// 使用例：AudioBuffer<int16_t, 512> 512samples
template <typename T, size_t samples>
using AudioMonoBuffer = AudioBuffer<T, samples, 1>;

/** 
    Split channel→Interleave convertion.
    @param source Split channel samples
    @param dest Interleave array
    @param numSamples 
    @param numChannels 
*/
void interleaveSamples (const float** source, float* dest, const uint_fast32_t numSamples, const uint_fast32_t numChannels)
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
void deinterleaveSamples (const float* source, float** dest, const uint_fast32_t numSamples, const uint_fast32_t numChannels)
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