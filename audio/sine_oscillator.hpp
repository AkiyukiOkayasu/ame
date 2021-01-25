/** 
    @file sine_oscillator.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "../math/constants.hpp"
#include "../math/specialFunctions.hpp"
#include "dspHelpers.hpp"

#include <atomic>

namespace ame
{
/** 
    Sine wave oscillator.
    
    Generates a sine between -1.0~1.0.
*/
class SineOscillator
{
public:
    /** Create sine wave ocillator instance.
        @param sampleRate The sample rate that will be used for calclate the oscillator phase  increment.
        @param frequency Initial frequency
    */
    SineOscillator(const float sampleRate, const float frequency) : samplingPeriod(1.0f / sampleRate)
    {
        setFrequency(frequency);
    }
    ~SineOscillator() {}

    /** Set the sine wave frequency
        @param freq frequency in Hz
    */
    void setFrequency(const float freq) noexcept { phaseIncrement = freq * twoPi * samplingPeriod; }

    /** Generate single sample
        @return generated latest sample
    */
    float nextSample() noexcept
    {
        phase = addModulo2Pi(phase, phaseIncrement);
        return ame::sinf(phase);
    }

private:
    float samplingPeriod;// サンプリング周期 1 /sampleRate
    std::atomic<float> phaseIncrement {0.0f};
    std::atomic<float> phase {0.0f};

    // Disallow copy constructor and assignment
    SineOscillator(const SineOscillator&) = delete;
    SineOscillator& operator=(const SineOscillator&) = delete;
};
}// namespace ame
