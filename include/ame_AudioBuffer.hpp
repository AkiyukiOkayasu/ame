/** 
    Audio buffer.
    @file ame_AudioBuffer.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <span>

namespace ame
{
/** 
    A lightweight data structure that stores a pointer to an audio buffer.

    This class doesn't own any of the data which it points to, it's simply a view into data that is owned elsewhere.
    You can construct one from some raw data that you've allocated yourself, or give it an AudioBuffer which it can refer to, but in all cases the user is responsible for making sure that the data doesn't get deleted while there's still an AudioBlock using it.    
    @attention Channel order is interleaved.
    @see AudioBuffer
*/
template <class ElementType, size_t Extent>
class AudioBlockView
{
public:
    /** Constructor.
        @param buffer Interleaved audio buffer view.
        @param numChannels the number of channels.
        @param numSamples the number of samples in each of the buffer's channels.        
        @attention The order of ame's audio buffers is interleaved. Note that it is NOT channel-splitting.
    */
    AudioBlockView (std::span<ElementType, Extent> view, const uint_fast32_t numChannels) noexcept
        : view (view),
          numChannels (numChannels)
    {
        numSamplesPerChannel = view.size() / numChannels;
    }
    ~AudioBlockView() = default;

    ///Returns the number of channels.
    uint_fast32_t getNumChannels() const noexcept
    {
        return numChannels;
    }

    ///Returns the number of samples per channel.
    uint_fast32_t getNumSamplesPerChannel() const noexcept
    {
        return numSamplesPerChannel;
    }

    /** Set a sample in the buffer.    
    @param destChannel 
    @param destSample 
    @param value     
    */
    void setSample (const uint_fast32_t destChannel, const uint_fast32_t destSample, const ElementType newValue)
    {
        assert (destChannel < numChannels);
        assert (destSample < numSamplesPerChannel);
        view[destSample * numChannels + destChannel] = newValue;
    }

    /** Add a value to a sample in the buffer.
    @param destChannel 
    @param destSample 
    @param valueToAdd 
    */
    void addSample (const uint_fast32_t destChannel, const uint_fast32_t destSample, const ElementType valueToAdd)
    {
        assert (destChannel < numChannels);
        assert (destSample < numSamplesPerChannel);
        view[destSample * numChannels + destChannel] += valueToAdd;
    }

    ///Set all samples to 0.
    void clear()
    {
        std::fill (view.begin(), view.end(), ElementType (0.0));
    }

    ///Applies a gain multiple to all the audio data.
    void applyGain (const float gain)
    {
        for_each (view.begin(), view.end(), [&gain] (auto& x)
                  { x *= gain; });
    }

    /** Finds the highest absolute sample value within a region of a channel.    
        @param channel 
        @return FloatType [0.0, FloatTypeMax]
    */
    float getPeak (const uint_fast32_t channel) const
    {
        float peak = 0.0f;

        for (auto i = channel; i < view.size(); i += numChannels)
        {
            const auto v = std::abs (view[i]);
            if (v > peak)
            {
                peak = v;
            }
        }
        return peak;
    }

    /** Returns the root mean squared level for a region of a channel.
    @param channel 
    @return FloatType RMS level [0.0, SampleTypeMax]
    */
    float getRMSLevel (const uint_fast32_t channel) const
    {
        assert (channel < numChannels);
        float sum = 0.0f;
        for (auto i = channel; i < view.size(); i += numChannels)
        {
            const auto sample = view[i];
            sum += sample * sample;
        }

        return std::sqrt (sum / numSamplesPerChannel);
    }

    auto subView (uint_fast32_t offset, uint_fast32_t size)
    {
        return AudioBlockView { view.subspan (offset, size), numChannels };
    }

    std::span<ElementType, Extent> view;

private:
    uint_fast32_t numSamplesPerChannel;
    uint_fast32_t numChannels;
};

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
    explicit AudioBuffer (const uint_fast32_t numChannels) : numChannels (numChannels), numSamplesPerChannel (Size / numChannels)
    {
    }
    ~AudioBuffer() = default;

    /**         
        Set the number of channels. Update numSamples (number of samples per channel) based on the buffer length.
        @param channels number of channels
    */
    void setNumChannels (const uint_fast32_t channels)
    {
        numChannels = channels;
        numSamplesPerChannel = Size / numChannels;
    }

    ///Returns the number of channels.
    uint_fast32_t getNumChannels() const noexcept
    {
        return numChannels;
    }

    ///Returns the number of samples per channel.
    uint_fast32_t getNumSamplesPerChannel() const noexcept
    {
        return numSamplesPerChannel;
    }

    ///Set all samples to 0.
    void clear()
    {
        buffer.fill (0.0f);
    }

    ///Applies a gain multiple to all the audio data.
    void applyGain (const float gain)
    {
        std::for_each (buffer.begin(), buffer.end(), [&gain] (auto& e)
                       { e *= gain; });
    }

    auto makeAudioBlockView (uint_fast32_t offset = 0, uint_fast32_t size = Size)
    {
        std::span sp { buffer.data() + offset, size };
        return ame::AudioBlockView { sp, numChannels };
    }

    ///addBuffer
    template <size_t Extent>
    void addFrom (const uint_fast32_t destChannel, const uint_fast32_t destStartSample, const AudioBlockView<FloatType, Extent>& source, const uint_fast32_t sourceChannel, const uint_fast32_t sourceStartSample, const uint_fast32_t numSamplesToAdd)
    {
        const auto& destOffset = destStartSample * numChannels;
        const auto& sourceOffset = sourceStartSample * source.getNumChannels();
        const auto& destEnd = destOffset + numSamplesToAdd * numChannels;
        const auto& sourceEnd = sourceOffset + numSamplesToAdd * source.getNumChannels();

        assert (destEnd <= Size);
        assert (sourceEnd <= source.getSize());

        for (uint_fast32_t i = 0; i < numSamplesToAdd; ++i)
        {
            buffer[destOffset + i * destChannel] += source.buffer[sourceOffset + i * source.getNumChannels];
        }
    }

    /** Finds the highest absolute sample value within a region of a channel.
        @param channel 
        @return FloatType [0.0, FloatTypeMax]
    */
    FloatType getPeak (const uint_fast32_t channel) const
    {
        assert (channel < numChannels);
        FloatType peak = 0.0;
        for (auto i = channel; i < numSamplesPerChannel * numChannels; i += numChannels)
        {
            const auto v = std::abs (buffer[i]);
            if (v > peak)
            {
                peak = v;
            }
        }
        return peak;
    }

    /** Returns the root mean squared level for a region of a channel.
    @param channel 
    @return FloatType RMS level [0.0, FloatTypeMax]
    */
    FloatType getRMSLevel (const uint_fast32_t channel) const
    {
        assert (channel < numChannels);
        FloatType sum = 0.0;
        for (auto i = channel; i < numSamplesPerChannel * numChannels; i += numChannels)
        {
            const auto sample = buffer[i];
            sum += sample * sample;
        }

        return static_cast<FloatType> (std::sqrt (sum / numSamplesPerChannel));
    }

    std::array<FloatType, Size> buffer = {};

private:
    uint_fast32_t numChannels;
    uint_fast32_t numSamplesPerChannel;
};
} // namespace ame
