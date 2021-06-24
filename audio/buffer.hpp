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
    Audio buffer pointer holder.
*/
class AudioBlock
{
public:
    /** Constructor.
        @param buffer Interleaved audio buffer.
        @param numSamples the number of samples in each of the buffer's channels.
        @param numChannels the number of channels.
        @attention The order of ame's audio buffers is interleaved. Note that it is NOT channel-splitting.
    */
    AudioBlock (float* buffer, const uint_fast32_t numSamples, const uint_fast32_t numChannels) noexcept
        : buffer (buffer),
          numSamples (numSamples),
          numChannels (numChannels)
    {
    }
    ~AudioBlock() = default;

    /** Returns a read only pointer to interleaved audio buffer.
        @return const float* interleaved samples
    */
    const float* getReadPointer() const noexcept
    {
        return buffer;
    }

    /** Returns a writeable pointer to interleaved audio buffer.
        @return float* interleaved audio buffer
    */
    float* getWritePointer() noexcept
    {
        return buffer;
    }

    /** Returns the number of samples allocated in each of the buffer's channels.
        @return uint_fast32_t 
    */
    uint_fast32_t getNumSamples() const noexcept
    {
        return numSamples;
    }

    /** Returns the numner of channels of audio data that this buffer contains.
        @return uint_fast32_t 
    */
    uint_fast32_t getNumChannels() const noexcept
    {
        return numChannels;
    }

private:
    float* buffer;
    uint_fast32_t numSamples;
    uint_fast32_t numChannels;
};
} // namespace ame