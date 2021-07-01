/** 
    Audio buffer.
    @file buffer.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "audioBlockView.hpp"

#include <array>

namespace ame
{
/** 
    An audio buffer that supports multiple channels whose size is determined at compile time.
    @attention Channel order is interleaved.
    @tparam SampleType
    @tparam Size Total length of buffer
*/
template <typename FloatType, size_t Size>
class AudioBuffer
{
public:
    AudioBuffer (const uint_fast32_t numChannels) : numChannels (numChannels)
    {
        numSamples = Size / numChannels;
    }
    ~AudioBuffer() = default;

    /**         
        Set the number of channels. Update numSamples (number of samples per channel) based on the buffer length.
        @param channels number of channels
    */
    void setNumChannels (const uint_fast32_t channels)
    {
        numChannels = channels;
        numSamples = Size / numChannels;
    }

    ///Returns the number of channels.
    uint_fast32_t getNumChannels() const noexcept
    {
        return numChannels;
    }

    ///Returns the number of samples per channel.
    uint_fast32_t getNumSamples() const noexcept
    {
        return numSamples;
    }

    /** Returns the length of the buffer.         
        @attention This is the total number of samples allocated to the buffer. It is NOT the number of samples per channel.
    */
    uint_fast32_t getSize() const noexcept
    {
        return Size;
    }

    /** Returns a read only pointer to interleaved audio buffer.
        @return const float* interleaved samples
    */
    const FloatType* getReadPointer() const noexcept
    {
        return buffer.data();
    }

    /** Returns a writeable pointer to interleaved audio buffer.
        @return float* interleaved audio buffer
    */
    FloatType* getWritePointer() noexcept
    {
        return buffer.data();
    }

    ///Set all samples to 0.
    void clear()
    {
        buffer.fill (0.0f);
    }

    ///Applies a gain multiple to all the audio data.
    void applyGain (const float gain)
    {
        for (auto& e : buffer)
        {
            e *= gain;
        }
    }

    {
    }

    ///addBuffer
    void addFrom (const uint_fast32_t destChannel, const uint_fast32_t destStartSample, const AudioBlockView<FloatType>& source, const uint_fast32_t sourceChannel, const uint_fast32_t sourceStartSample, const uint_fast32_t numSamplesToAdd)
    {
        const auto& destOffset = destStartSample * numChannels;
        const auto& sourceOffset = sourceStartSample * source.getNumChannels();
        [[maybe_unused]] const auto& destEnd = destOffset + numSamplesToAdd * numChannels;
        [[maybe_unused]] const auto& sourceEnd = sourceOffset + numSamplesToAdd * source.getNumChannels();

        assert (destEnd <= Size);
        assert (sourceEnd <= source.getSize());

        for (uint_fast32_t i = 0; i < numSamplesToAdd; ++i)
        {
            buffer[destOffset + i * destChannel] += source.buffer[sourceOffset + i * source.getNumChannels];
        }
    }

private:
    std::array<FloatType, Size> buffer = {};
    uint_fast32_t numChannels;
    uint_fast32_t numSamples;
};
} // namespace ame
