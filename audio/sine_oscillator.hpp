// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// Sine wave oscillator. Generates a sine between -1.0~1.0.

#pragma once

#include "../math/constants.hpp"
#include "dspHelpers.hpp"

#if __has_include("arm_math.h")
#include "arm_math.h"
#else
#include "math.h"
#endif

namespace ame
{
    class SineOscillator
    {
    public:
        SineOscillator(const float sampleRate, const float frequency)
            : samplingPeriod(1.0f / sampleRate)
        {
            setFrequency(frequency);
        };
        ~SineOscillator(){};

        void setFrequency(const float freq) noexcept
        {
            phaseIncrement = freq * twoPi * samplingPeriod;
        }

        float nextSample() noexcept
        {
            phase = addModulo2Pi(phase, phaseIncrement);
#if __has_include("arm_math.h")
            return arm_sin_f32(phase);
#else
            return sinf(phase);
#endif
        }

    private:
        float samplingPeriod; // サンプリング周期: 1 /sampleRate
        float phaseIncrement;
        float phase = 0.0f;

        // Disallow copy constructor and assignment
        SineOscillator(const SineOscillator &) = delete;
        SineOscillator &operator=(const SineOscillator &) = delete;
    };
} // namespace ame
