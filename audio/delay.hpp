/** 
    Fractional delay.    
    @file delay.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "../math/constants.hpp"
#include "../system/wrap.hpp"
#include "audioBlockView.hpp"
#include "interpolation.hpp"

#include <array>

namespace ame::dsp
{
/** Fractional delay.
    @tparam maximumChannels 
    @tparam maximumDelayInSamples 
*/
template <size_t maximumChannels, size_t maximumDelayInSamples>
class Delay
{
public:
    Delay() = default;
    ~Delay() = default;

    /** Set delay time.
        A fractional delay can also be set.
        @param delayInSamples [0. maximumDelayInSamples]
    */
    void setDelay (const float delayInSamples)
    {
        assert (delayInSamples <= maximumDelayInSamples);
        assert (0 <= delayInSamples);

        fractional = delayInSamples - static_cast<uint_fast32_t> (delayInSamples);
        readPos.set (writePos.get (-delayInSamples));
    }

    /// Process audio effect.
    template <typename SampleType>
    void process (AudioBlockView<SampleType>& block)
    {
        assert (block.getNumChannels() <= maximumChannels);

        auto buffer = block.getWritePointer();
        uint_fast32_t i = 0;
        for (uint_fast32_t samp = 0; samp < block.getNumSamples(); ++samp)
        {
            for (uint_fast32_t ch = 0; ch < block.getNumChannels(); ++ch)
            {
                const float input = buffer[i];
                delayLine[ch][writePos.get()] = input;
                buffer[i] = lerp (delayLine[ch][readPos.get()], delayLine[ch][readPos.get (-1)], fractional);
                ++i;
            }
            ++readPos;
            ++writePos;
        }
    }

private:
    std::array<std::array<float, maximumDelayInSamples + 1>, maximumChannels> delayLine {}; ///<Ring buffer
    Wrap<maximumDelayInSamples + 1> readPos;                                                ///< Ring buffer read position
    Wrap<maximumDelayInSamples + 1> writePos;                                               ///< Ring buffer write position
    float fractional = 0.0f;                                                                ///<for fractional delay[0, 1]
};
} // namespace ame::dsp
