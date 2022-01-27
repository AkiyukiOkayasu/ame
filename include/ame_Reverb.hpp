/**
    Reverb.
    @file ame_Reverb.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    AME is released under the MIT license.
*/

#pragma once

#include "ame_AudioBuffer.hpp"
#include "ame_Util.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <type_traits>

namespace ame::dsp
{
/** Reverb.
    @tparam FloatType float or double
    @tparam MaximumSampleRate 
*/
template <typename FloatType, size_t MaximumChannels, size_t MaximumSampleRate>
class Freeverb
{
    static_assert (std::is_floating_point<FloatType>::value, "FloatType is must be floating point type.");

public:
    explicit Freeverb (const FloatType sampleRate)
    {
        setSampleRate (sampleRate);
        setRoomSize (0.5f, 0.5f);
    }
    ~Freeverb() = default;

    /** Room size and damping.
        @param roomSize 1.0 is big, 0 is small [0.0 1.0]
        @param damp 0 is not damped, 1.0 is fully damped [0.0 1.0]
    */
    void setRoomSize (const FloatType roomSize, const FloatType damp) noexcept
    {
        static constexpr FloatType roomScaleFactor = 0.28f;
        static constexpr FloatType roomOffset = 0.7f;
        static constexpr FloatType dampScaleFactor = 0.4f;
        damping = damp * dampScaleFactor;
        feedback = roomSize * roomScaleFactor + roomOffset;
    }

    /** Sets the sample rate that will be used for the reverb.
        You must call this before the process methods, in order to tell it the correct sample rate.
    */
    void setSampleRate (const FloatType sampleRate)
    {
        assert (0 < sampleRate);
        assert (sampleRate <= MaximumSampleRate);

        static constexpr int_fast16_t combTunings[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; ///< (at 44100Hz)
        static constexpr int_fast16_t allPassTunings[] = { 556, 441, 341, 225 };                          ///< (at 44100Hz)
        static constexpr FloatType sampleRateRatio = sampleRate / 44100.0f;

        for (int i = 0; i < numCombs; ++i)
        {
            comb[0][i].setSize (std::round (combTunings[i] * sampleRateRatio));
            comb[1][i].setSize (std::round ((combTunings[i] + stereoSpread) * sampleRateRatio));
        }

        for (int i = 0; i < numAllPasses; ++i)
        {
            allPass[0][i].setSize (std::round (allPassTunings[i] * sampleRateRatio));
            allPass[1][i].setSize (std::round ((allPassTunings[i] + stereoSpread) * sampleRateRatio));
        }

        const int_fast16_t smoothSteps = static_cast<int_fast16_t> (sampleRate * 0.01f); //10ms
        dryWet.setRampLength (smoothSteps);
    }

    /** Dry/Wet balance.
        @param mix 0: dry only, 1: wet only [0.0, 1.0]
    */
    void setDryWet (const FloatType mix)
    {
        dryWet.setTargetValue (mix);
    }

    /** Clears the reverb's buffers. */
    void reset()
    {
        for (int ch = 0; ch < MaximumChannels; ++ch)
        {
            for (int i = 0; i < numCombs; ++i)
            {
                comb[ch][i].clear();
            }

            for (int i = 0; i < numAllPasses; ++i)
            {
                allPass[ch][i].clear();
            }
        }
    }

    //==============================================================================
    /** Applies the reverb to two stereo channels of audio data. */
    template <size_t N>
    void process (AudioBlockView<FloatType, N> block)
    {
        const uint_fast32_t numChannels = block.getNumChannels();
        const uint_fast32_t bufferSize = block.getNumSamplesPerChannel();
        assert (numChannels <= MaximumChannels);

        block.applyGain (inputGain);

        uint_fast32_t i = 0;
        for (uint_fast32_t samp = 0; samp < bufferSize; ++samp)
        {
            const FloatType mix = dryWet.getNextValue();
            for (uint_fast32_t ch = 0; ch < numChannels; ++ch)
            {
                const FloatType input = block.view[i];
                FloatType output = 0.0f;

                for (int cmb = 0; cmb < numCombs; ++cmb) //accumulate the comb filters in parallel
                {
                    output += comb[ch][cmb].process (input, damping, feedback);
                }

                for (int ap = 0; ap < numAllPasses; ++ap) // run the allpass filters in series
                {
                    output = allPass[ch][ap].process (output);
                }

                block.view[i] = std::lerp (input, output, mix);
                ++i;
            }
        }
    }

private:
    static constexpr int stereoSpread = 23;
    static constexpr FloatType inputGain = 0.015f;
    static constexpr int numCombs = 8;
    static constexpr int numAllPasses = 4;

    class CombFilter
    {
    public:
        CombFilter() noexcept {}

        void setSize (const int size)
        {
            assert (size <= buffer.size());
            bufferIndex.changeLength (size);
            bufferIndex.set (0);
            clear();
        }

        void clear() noexcept
        {
            last = 0;
            buffer.fill (0.0f);
        }

        FloatType process (const FloatType input, const FloatType damp, const FloatType feedbackLevel) noexcept
        {
            const auto ind = bufferIndex.get();
            const FloatType output = buffer[ind];
            last = (output * (1.0f - damp)) + (last * damp);
            buffer[ind] = input + (last * feedbackLevel);
            bufferIndex++;
            return output;
        }

    private:
        static constexpr int bufferAllocatedSize = (1617 + stereoSpread) * (MaximumSampleRate / 44100.0f);
        std::array<FloatType, bufferAllocatedSize> buffer {};
        ame::Wrap<int> bufferIndex { bufferAllocatedSize };
        FloatType last {};
    };

    //==============================================================================
    class AllPassFilter
    {
    public:
        AllPassFilter() noexcept {}

        void setSize (const int size)
        {
            bufferIndex.changeLength (size);
            bufferIndex.set (0);
            clear();
        }

        void clear() noexcept
        {
            buffer.fill (0.0f);
        }

        FloatType process (const FloatType input) noexcept
        {
            const auto ind = bufferIndex.get();
            const FloatType bufferedValue = buffer[ind];
            buffer[ind] = input + (bufferedValue * 0.5f);
            bufferIndex++;
            return bufferedValue - input;
        }

    private:
        static constexpr int bufferAllocatedSize = (556 + stereoSpread) * (MaximumSampleRate / 44100.0f);
        std::array<FloatType, bufferAllocatedSize> buffer;
        ame::Wrap<int> bufferIndex { bufferAllocatedSize };
    };

    CombFilter comb[MaximumChannels][numCombs] {};
    AllPassFilter allPass[MaximumChannels][numAllPasses] {};
    LinearSmoothedValue<FloatType> dryWet { 0.5f, 100 };
    FloatType damping {};
    FloatType feedback {};
};
} // namespace ame::dsp
