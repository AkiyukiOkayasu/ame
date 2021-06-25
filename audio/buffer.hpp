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
    @tparam Capacity
    @tparam NumSamples the number of samples in each of the buffer's channels.
*/
template <typename SampleType, size_t Capacity>
class AudioBuffer
{
public:
    AudioBuffer (const uint_fast32_t numChannels, const uint_fast32_t numSamples) : numChannels (numChannels), numSamples (numSamples)
    {
		assert (Capacity >= numChannels * numSamples);//Capacity should be equal to or greater than numChannels * numSamples.
    }
    ~AudioBuffer() = default;

    uint_fast32_t getNumChannels() const noexcept
    {
        return numChannels;
    }

    uint_fast32_t getNumSamples() const noexcept
    {
        return numSamples;
    }

    const SampleType* getReadPointer() const noexcept
    {
        return buffer.data();
    }

    SampleType* getWritePointer() noexcept
    {
        return buffer.data();
    }

    void clear()
    {
        buffer.fill (0.0);
    }

private:
    std::array<SampleType, Capacity> buffer = {};
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
