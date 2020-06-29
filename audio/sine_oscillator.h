// Copyright 2020 Akiyuki Okayasu.
//
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
//
// -------------------------------------------------------
//
// Sine wave oscillator. Generates a sine between -1.0~1.0.

#ifndef _NXPLIB_SINE_OSCILLATOR_H_
#define _NXPLIB_SINE_OSCILLATOR_H_

#include "../math/constants.h"
#include "dspHelpers.h"

#include "arm_math.h"

namespace nxplib
{
    class SineOscillator
    {
    public:
        SineOscillator(const float sampleRate, const float frequency)
            : samplingPeriod(1.0f/sampleRate)
        {
            setFrequency(frequency);
        };
        ~SineOscillator(){};

        void setFrequency(const float freq) noexcept
        {
            phaseIncrement = freq * twoPi<float> * samplingPeriod;
        }

        float nextSample() noexcept
        {
            phase = addModulo2Pi(phase, phaseIncrement);
            return arm_sin_f32(phase);
        }

    private:
        float samplingPeriod;// サンプリング周期: 1 /sampleRate
        float phaseIncrement;
        float phase = 0.0f;

        // Disallow copy constructor and assignment
        SineOscillator(const SineOscillator &) = delete;
        SineOscillator &operator=(const SineOscillator &) = delete;
    };
} // namespace nxplib

#endif //_NXPLIB_SINE_OSCILLATOR_H_
