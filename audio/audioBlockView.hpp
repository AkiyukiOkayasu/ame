/** 
    Audio Block View.
    @file audioBlockView.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <cmath>
#include <cstdint>

namespace ame
{
/** 
    A lightweight data structure that stores a pointer to an audio buffer.

    This class doesn't own any of the data which it points to, it's simply a view into data that is owned elsewhere.
    You can construct one from some raw data that you've allocated yourself, or give it an AudioBuffer which it can refer to, but in all cases the user is responsible for making sure that the data doesn't get deleted while there's still an AudioBlock using it.    
    @attention Channel order is interleaved.
    @see AudioBuffer
*/
template <typename SampleType>
class AudioBlockView
{
public:
    /** Constructor.
        @param buffer Interleaved audio buffer.
        @param numChannels the number of channels.
        @param numSamples the number of samples in each of the buffer's channels.        
        @attention The order of ame's audio buffers is interleaved. Note that it is NOT channel-splitting.
    */
    AudioBlockView (SampleType* buffer, const uint_fast32_t numChannels, const uint_fast32_t numSamples) noexcept
        : buffer (buffer),
          numSamples (numSamples),
          numChannels (numChannels)
    {
    }
    ~AudioBlockView() = default;

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
        return numChannels * numSamples;
    }

    /** Returns a read only pointer to interleaved audio buffer.
        @return const float* interleaved samples
    */
    const SampleType* getReadPointer() const noexcept
    {
        return buffer;
    }

    /** Returns a writeable pointer to interleaved audio buffer.
        @return float* interleaved audio buffer
    */
    SampleType* getWritePointer() noexcept
    {
        return buffer;
    }

    ///Set all samples to 0.
    void clear()
    {
        uint_fast32_t i = 0;
        for (uint_fast32_t samp = 0; samp < numSamples; ++samp)
        {
            for (uint_fast32_t ch = 0; ch < numChannels; ++ch)
            {
                buffer[i] = static_cast<SampleType> (0.0f);
                ++i;
            }
        }
    }

    ///Applies a gain multiple to all the audio data.
    void applyGain (const float gain)
    {
        uint_fast32_t i = 0;
        for (uint_fast32_t samp = 0; samp < numSamples; ++samp)
        {
            for (uint_fast32_t ch = 0; ch < numChannels; ++ch)
            {
                buffer[i] *= gain;
                ++i;
            }
        }
    }

    /**
    
        @param channel 
        @return FloatType [0.0, FloatTypeMax]
    */
    SampleType getPeak (const uint_fast32_t channel) const
    {
        SampleType peak = 0.0;
        for (auto i = channel; i < numSamples * numChannels; i += numChannels)
        {
            const auto v = std::abs (buffer[i]);
            if (v > peak)
            {
                peak = v;
            }
        }
        return peak;
    }

private:
    SampleType* buffer;
    uint_fast32_t numSamples;
    uint_fast32_t numChannels;
};
template class AudioBlockView<float>; //明示的テンプレートのインスタンス化
} // namespace ame