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
        setParameters (Parameters());
        setSampleRate (sampleRate);
    }
    ~Freeverb() = default;

    //==============================================================================
    /// Holds the parameters being used by a Reverb object.
    struct Parameters
    {
        FloatType roomSize = 0.5;   ///< Room size, 0 to 1.0, where 1.0 is big, 0 is small.
        FloatType damping = 0.5;    ///< Damping, 0 to 1.0, where 0 is not damped, 1.0 is fully damped.
        FloatType wetLevel = 0.33;  ///< Wet level, 0 to 1.0
        FloatType dryLevel = 0.4;   ///< Dry level, 0 to 1.0
        FloatType width = 1.0;      ///< Reverb width, 0 to 1.0, where 1.0 is very wide.
        FloatType freezeMode = 0.0; ///< Freeze mode - values < 0.5 are "normal" mode, values > 0.5 put the reverb into a continuous feedback loop.
    };

    //==============================================================================
    /// Returns the reverb's current parameters.
    const Parameters& getParameters() const noexcept { return parameters; }

    /** Applies a new set of parameters to the reverb.
        @note If you call this in parallel with the process method, you may get artifacts.
    */
    void setParameters (const Parameters& newParams)
    {
        static constexpr FloatType wetScaleFactor = 3.0;
        static constexpr FloatType dryScaleFactor = 2.0;

        const FloatType wet = newParams.wetLevel * wetScaleFactor;
        dryGain.setTargetValue (newParams.dryLevel * dryScaleFactor);
        wetGain1.setTargetValue (FloatType (0.5) * wet * (1.0 + newParams.width));
        wetGain2.setTargetValue (FloatType (0.5) * wet * (1.0 - newParams.width));

        gain = isFrozen (newParams.freezeMode) ? 0.0f : 0.015f;
        parameters = newParams;
        updateDamping();
    }

    //==============================================================================
    /** Sets the sample rate that will be used for the reverb.
        You must call this before the process methods, in order to tell it the correct sample rate.
    */
    void setSampleRate (const FloatType sampleRate)
    {
        assert (0 < sampleRate);
        assert (sampleRate <= MaximumSampleRate);

        static const short combTunings[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; ///< (at 44100Hz)
        static const short allPassTunings[] = { 556, 441, 341, 225 };                          ///< (at 44100Hz)
        const int intSampleRate = (int) sampleRate;

        for (int i = 0; i < numCombs; ++i)
        {
            comb[0][i].setSize ((intSampleRate * combTunings[i]) / 44100);
            comb[1][i].setSize ((intSampleRate * (combTunings[i] + stereoSpread)) / 44100);
        }

        for (int i = 0; i < numAllPasses; ++i)
        {
            allPass[0][i].setSize ((intSampleRate * allPassTunings[i]) / 44100);
            allPass[1][i].setSize ((intSampleRate * (allPassTunings[i] + stereoSpread)) / 44100);
        }

        const int smoothSteps = 0.01 * sampleRate;
        damping.setRampLength (smoothSteps);
        feedback.setRampLength (smoothSteps);
        dryGain.setRampLength (smoothSteps);
        wetGain1.setRampLength (smoothSteps);
        wetGain2.setRampLength (smoothSteps);
    }

    /** Clears the reverb's buffers. */
    void reset()
    {
        for (int j = 0; j < MaximumChannels; ++j)
        {
            for (int i = 0; i < numCombs; ++i)
            {
                comb[j][i].clear();
            }

            for (int i = 0; i < numAllPasses; ++i)
            {
                allPass[j][i].clear();
            }
        }
    }

    //==============================================================================
    /** Applies the reverb to two stereo channels of audio data. */
    template <size_t N>
    void process (AudioBlockView<FloatType, N> block)
    {
        assert (block.getNumChannels() <= MaximumChannels);

        uint_fast32_t i = 0;
        for (uint_fast32_t samp = 0; samp < block.getNumSamplesPerChannel(); ++samp)
        {
            for (uint_fast32_t ch = 0; ch < block.getNumChannels(); ++ch)
            {
                const FloatType input = block.view[i] * gain;
                FloatType output {};
                const FloatType damp = damping.getNextValue();
                const FloatType feedbck = feedback.getNextValue();

                for (int cmb = 0; cmb < numCombs; ++cmb) //accumulate the comb filters in parallel
                {
                    output += comb[ch][cmb].process (input, damp, feedbck);
                }

                for (int ap = 0; ap < numAllPasses; ++ap) // run the allpass filters in series
                {
                    output = allPass[ch][ap].process (output);
                }

                const FloatType dry = dryGain.getNextValue();
                const FloatType wet = wetGain1.getNextValue();

                block.view[i] = output * wet + input * dry;
                ++i;
            }
        }
    }

private:
    static constexpr int stereoSpread = 23;

    //==============================================================================
    static bool isFrozen (const FloatType freezeMode) noexcept
    {
        return freezeMode >= 0.5f;
    }

    void updateDamping() noexcept
    {
        static constexpr FloatType roomScaleFactor = 0.28;
        static constexpr FloatType roomOffset = 0.7;
        static constexpr FloatType dampScaleFactor = 0.4;

        if (isFrozen (parameters.freezeMode))
        {
            setDamping (0.0f, 1.0f);
        }
        else
        {
            setDamping (parameters.damping * dampScaleFactor,
                        parameters.roomSize * roomScaleFactor + roomOffset);
        }
    }

    void setDamping (const float dampingToUse, const float roomSizeToUse) noexcept
    {
        damping.setTargetValue (dampingToUse);
        feedback.setTargetValue (roomSizeToUse);
    }

    //==============================================================================
    class CombFilter
    {
    public:
        CombFilter() noexcept {}

        void setSize (const int size)
        {
            assert (size <= buffer.size());

            if (size != bufferSize)
            {
                bufferIndex = 0;
                bufferSize = size;
            }

            clear();
        }

        void clear() noexcept
        {
            last = 0;
            buffer.fill (0.0);
        }

        FloatType process (const FloatType input, const FloatType damp, const FloatType feedbackLevel) noexcept
        {
            const FloatType output = buffer[bufferIndex];
            last = (output * FloatType (1.0 - damp)) + (last * damp);
            //JUCE_UNDENORMALISE (last);

            const FloatType temp = input + (last * feedbackLevel);
            //JUCE_UNDENORMALISE (temp);
            buffer[bufferIndex] = temp;
            bufferIndex = (bufferIndex + 1) % bufferSize;
            return output;
        }

    private:
        static constexpr int bufferAllocatedSize = (1617 + stereoSpread) * (MaximumSampleRate / 44100.0);
        std::array<FloatType, bufferAllocatedSize> buffer {};
        int bufferIndex = 0;
        int bufferSize = 0;
        FloatType last = 0.0;
    };

    //==============================================================================
    class AllPassFilter
    {
    public:
        AllPassFilter() noexcept {}

        void setSize (const int size)
        {
            if (size != bufferSize)
            {
                bufferIndex = 0;
                bufferSize = size;
            }

            clear();
        }

        void clear() noexcept
        {
            buffer.fill (0.0);
        }

        FloatType process (const FloatType input) noexcept
        {
            const FloatType bufferedValue = buffer[bufferIndex];
            float temp = input + (bufferedValue * 0.5f);
            //JUCE_UNDENORMALISE (temp);
            buffer[bufferIndex] = temp;
            bufferIndex = (bufferIndex + 1) % bufferSize;
            return bufferedValue - input;
        }

    private:
        static constexpr int bufferAllocatedSize = (556 + stereoSpread) * (MaximumSampleRate / 44100.0);
        std::array<FloatType, bufferAllocatedSize> buffer;
        int bufferSize = 0;
        int bufferIndex = 0;
    };

    //==============================================================================
    Parameters parameters;
    FloatType gain {};
    static constexpr int numCombs = 8;
    static constexpr int numAllPasses = 4;

    CombFilter comb[MaximumChannels][numCombs];
    AllPassFilter allPass[MaximumChannels][numAllPasses];

    static constexpr int rampLengthInSamples = MaximumSampleRate * 0.01;
    LinearSmoothedValue<FloatType> damping { 0.5, rampLengthInSamples };
    LinearSmoothedValue<FloatType> feedback { 0.0, rampLengthInSamples };
    LinearSmoothedValue<FloatType> dryGain { 0.4, rampLengthInSamples };
    LinearSmoothedValue<FloatType> wetGain1 { 0.33, rampLengthInSamples };
    LinearSmoothedValue<FloatType> wetGain2 { 0.33, rampLengthInSamples };
};
} // namespace ame::dsp