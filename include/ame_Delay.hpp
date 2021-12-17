/**
    Fractional delay.
    @file ame_Delay.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    AME is released under the MIT license.
*/

#pragma once

#include "ame_AudioBuffer.hpp"
#include "ame_Math.hpp"
#include "ame_Util.hpp"

#include <array>
#include <cassert>
#include <cmath>

namespace ame::dsp
{
/** Fractional delay.
    @tparam maximumChannels
    @tparam maximumDelayInSamples
*/
template <size_t MaximumChannels, size_t MaximumDelayInSamples>
class Delay
{
public:
    Delay()
    {
        readPos.changeLength (MaximumChannels * MaximumDelayInSamples);
        writePos.changeLength (MaximumChannels * MaximumDelayInSamples);
    }
    ~Delay() = default;

    /** Set delay time.
        A fractional delay can also be set.
        @param delayInSamples [0. maximumDelayInSamples]
    */
    void setDelay (const float delayInSamples)
    {
        assert (delayInSamples <= MaximumDelayInSamples);
        assert (0 <= delayInSamples);

        fractional = delayInSamples - static_cast<uint_fast32_t> (delayInSamples);
        readPos.set (writePos.get (-delayInSamples));
    }

    /// Process audio effect.
    template <typename SampleType, size_t N>
    void process (AudioBlockView<SampleType, N>& block)
    {
        assert (block.getNumChannels() <= MaximumChannels);

        uint_fast32_t i = 0;
        for (uint_fast32_t samp = 0; samp < block.getNumSamplesPerChannel(); ++samp)
        {
            for (uint_fast32_t ch = 0; ch < block.getNumChannels(); ++ch)
            {
                const float input = block.view[i];
                delayLine[ch][writePos.get()] = input;
                block.view[i] = std::lerp (delayLine[ch][readPos.get()], delayLine[ch][readPos.get (-1)], fractional);
                ++i;
            }
            ++readPos;
            ++writePos;
        }
    }

private:
    std::array<std::array<float, MaximumDelayInSamples + 1>, MaximumChannels> delayLine {}; ///<Ring buffer
    Wrap<int32_t> readPos {};                                                               ///< Ring buffer read position
    Wrap<int32_t> writePos {};                                                              ///< Ring buffer write position
    float fractional = 0.0f;                                                                ///<for fractional delay[0, 1]
};
} // namespace ame::dsp
