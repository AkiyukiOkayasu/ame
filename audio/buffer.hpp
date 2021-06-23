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
inline void interleaveSamples (const float** source, float* dest, const uint_fast32_t numSamples, const uint_fast32_t numChannels)
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

/** 
    Audio buffer pointer holder.
*/
class AudioBlock
{
public:
    /** Constructor.
        @param buffer Interleaved samples
        @param numSamples 
        @param numChannels 
    */
    AudioBlock (float* buffer, const uint_fast32_t numSamples, const uint_fast32_t numChannels) noexcept
        : buffer (buffer),
          numSamples (numSamples),
          numChannels (numChannels)
    {
    }
    ~AudioBlock() = default;

    /** 
        Get readable buffer.
        @return const float* interleaved samples
    */
    const float* getReadPointer() const noexcept
    {
        return buffer;
    }

    /** 
        Get writable buffer.
        @return float* interleaved samples
    */
    float* getWritePointer() noexcept
    {
        return buffer;
    }

    uint_fast32_t getNumSamples() const noexcept
    {
        return numSamples;
    }

    uint_fast32_t getNumChannels() const noexcept
    {
        return numChannels;
    }
    ///@todo interleaveとchannel arrayのうまいやりとり追加検討

private:
    float* buffer;
    uint_fast32_t numSamples;
    uint_fast32_t numChannels;
};
} // namespace ame