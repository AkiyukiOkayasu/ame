/**
 * @file sine_oscillator.hpp
 * @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
 * @brief Sine wave oscillator.
 *
 * @copyright Copyright (c) 2021 Akiyuki Okayasu
 *
 * AME is released under the MIT license.
 */

#pragma once

#include "../math/constants.hpp"
#include "../math/specialFunctions.hpp"
#include "dspHelpers.hpp"
#include <atomic>

namespace ame
{
    /**
     * @brief Sine wave oscillator. Generates a sine between -1.0~1.0.
     *
     */
    class SineOscillator
    {
      public:
        SineOscillator(const float sampleRate, const float frequency) : samplingPeriod(1.0f / sampleRate)
        {
            setFrequency(frequency);
        }
        ~SineOscillator()
        {
        }

        /**
         * @brief Set the sine wave frequency
         * @param freq frequency in Hz
         */
        void setFrequency(const float freq) noexcept
        {
            phaseIncrement = freq * twoPi * samplingPeriod;
        }

        /**
         * @brief generate one sample
         * @return generated latest sample
         */
        float nextSample() noexcept
        {
            phase = addModulo2Pi(phase, phaseIncrement);
            return ame::sinf(phase);
        }

      private:
        float samplingPeriod; // サンプリング周期 1 /sampleRate
        std::atomic<float> phaseIncrement{0.0f};
        std::atomic<float> phase{0.0f};

        // Disallow copy constructor and assignment
        SineOscillator(const SineOscillator&) = delete;
        SineOscillator& operator=(const SineOscillator&) = delete;
    };
} // namespace ame
