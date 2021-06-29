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
/** 
    An audio buffer that supports multiple channels whose size is determined at compile time.
    @attention Channel order is interleaved.
    @tparam SampleType
    @tparam Size Total length of buffer
*/
template <typename SampleType, size_t Size>
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
    const SampleType* getReadPointer() const noexcept
    {
        return buffer.data();
    }

    /** Returns a writeable pointer to interleaved audio buffer.
        @return float* interleaved audio buffer
    */
    SampleType* getWritePointer() noexcept
    {
        return buffer.data();
    }

    ///Set all samples to 0.
    void clear()
    {
        buffer.fill (0.0);
    }

private:
    std::array<SampleType, Size> buffer = {};
    uint_fast32_t numChannels;
    uint_fast32_t numSamples;
};

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
    SampleType* buffer;
    uint_fast32_t numSamples;
    uint_fast32_t numChannels;
};
template class AudioBlockView<float>; //明示的テンプレートのインスタンス化
} // namespace ame
