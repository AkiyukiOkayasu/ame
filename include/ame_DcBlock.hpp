/**
    DC block filter.
    @file ame_DcBlock.hpp
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
#include <cstdint>

namespace ame::dsp
{
/** DC block filter.
    
    It works the same as dcblock operator used in gen~ in Cycling'74 Max.

    @tparam FloatType 
    @tparam MaximumChannels 
*/
template <typename FloatType, size_t MaximumChannels>
class DcBlock
{
public:
    DcBlock() = default;
    ~DcBlock() = default;

    /// Process audio effect.
    template <typename SampleType, size_t N>
    void process (AudioBlockView<SampleType, N>& block)
    {
        assert (block.getNumChannels() <= MaximumChannels);

        const auto numChannels = block.getNumChannels();
        const auto bufferSize = block.getNumSamplesPerChannel();

        uint_fast32_t i = 0;
        for (uint_fast32_t samp = 0; samp < bufferSize; ++samp)
        {
            for (uint_fast32_t ch = 0; ch < numChannels; ++ch)
            {
                const FloatType input = block.view[i];
                const FloatType y = input - x1[ch] + (y1[ch] * c);
                x1[ch] = input;
                y1[ch] = y;
                block.view[i] = y;
                ++i;
            }
        }
    }

private:
    /** Usually a value in the range of 0.9 to 1 is used.
        The same values as the dcblock operator in Cycling'74 Max are adopted.
    */
    static constexpr FloatType c = (FloatType) 0.9997;

    std::array<FloatType, MaximumChannels> x1 {};
    std::array<FloatType, MaximumChannels> y1 {};
};
} // namespace ame::dsp
