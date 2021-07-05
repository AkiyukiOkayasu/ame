/** 
    Delay.    

    @file delay.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "../math/constants.hpp"
#include "audioBlockView.hpp"
#include "interpolation.hpp"

#include <array>

namespace ame::dsp
{
template <size_t maximumChannels, size_t maximumDelayInSamples>
class Delay
{
public:
    Delay() = default;
    ~Delay() = default;

    /** Set delay time.
        @param delayInSamples 
    */
    void setDelay (const float delayInSamples)
    {
        assert (delayTimeInSamples <= maximumDelayTimeInSamples);
        readPos = wrap (writePos - delayTimeInSamples);
    }

    /// Process audio effect.
    template <typename SampleType>
    void process (AudioBlockView<SampleType>& block)
    {
        assert (block.getNumChannels <= maximumChannels);

        auto buffer = block.getWritePointer();
        uint_fast32_t i = 0;
        for (uint_fast32_t samp = 0; samp < block.getNumSamples(); ++samp)
        {
            for (uint_fast32_t ch = 0; ch < block.getNumChannels(); ++ch)
            {
                const float input = buffer[i];
                delayLine[ch][writePos] = input;
                buffer[i] = delayLine[ch][readPos];
                ++i;
            }
            ++readPos;
            ++writePos;
        }
    }

private:
    /// wrap value range 0 <= n < maximumDelaySamples.
    uint_fast32_t wrap (uint_fast32_t n)
    {
        if (n >= 0)
        {
            return n;
        }

        return n + maximumDelayTimeInSamples;
    }
    std::array<std::array<float, maximumDelayTimeInSamples>, maximumChannels> delayLine {};
    uint_fast32_t readPos = 0;
    uint_fast32_t writePos = 0;
};
} // namespace ame::dsp