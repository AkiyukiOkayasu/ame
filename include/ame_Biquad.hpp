/** 
    Biquad filter.
    Audio-EQ-Cookbook by Robert Bristow-Johnson implementation.
    https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

    @file ame_Biquad.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "ame_AudioBuffer.hpp"
#include "ame_Math.hpp"

#include <array>
#include <atomic>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>

namespace ame::dsp
{
/// BiQuad filter.
template <std::floating_point FloatType, size_t MaximumChannels>
class Biquad
{
public:
    explicit Biquad (const FloatType sampleRate)
        : sampleRate (sampleRate)
    {
    }
    ~Biquad() = default;

    /// Set new sampleRate.
    void setSampleRate (const FloatType newSampleRate) noexcept
    {
        sampleRate = newSampleRate;
    }

    /// Return sampleRate.
    FloatType getSampleRate() const noexcept
    {
        return sampleRate;
    }

    /// BiQuad filter coefficients.
    struct Coefficients
    {
        FloatType a0 = 0.0;
        FloatType a1 = 0.0;
        FloatType a2 = 0.0;
        FloatType b0 = 0.0;
        FloatType b1 = 0.0;
        FloatType b2 = 0.0;
    };

    /** Set BiQuad coefficients.
        @param c BiQuad coefficients        
        @code        
        filter.setCoefficients(makeLowPass(48000.0f, 440.0f, 0.707f));
        @endcode
    */
    void setCoefficients (const Coefficients& c) noexcept
    {
        coef = c;
    }

    Coefficients getCoefficients() const noexcept
    {
        return coef;
    }

    /// Low-pass coefficients calculation from RBJ cookbook.
    void makeLowPass (const FloatType cutOffFrequency, const FloatType Q) noexcept
    {
        const FloatType w0 = twoPi<FloatType> * cutOffFrequency / sampleRate;
        const FloatType alpha = sinf (w0) / (FloatType (2.0) * Q);

        coef.a0 = FloatType (1.0) + alpha;
        coef.b0 = (FloatType (1.0) - cosf (w0)) / FloatType (2.0);
        coef.b1 = FloatType (1.0) - cosf (w0);
        coef.b2 = coef.b0;
        coef.a1 = FloatType (-2.0) * cosf (w0);
        coef.a2 = FloatType (1.0) - alpha;
        normalizeCoefficientsByA0();
    }

    /// High-pass coefficients calculation from RBJ cookbook.
    void makeHighPass (const FloatType cutOffFrequency, const FloatType Q) noexcept
    {
        const FloatType w0 = twoPi<FloatType> * cutOffFrequency / sampleRate;
        const FloatType alpha = sinf (w0) / (FloatType (2.0) * Q);

        coef.a0 = FloatType (1.0) + alpha;
        coef.b0 = (FloatType (1.0) + cosf (w0)) / FloatType (2.0);
        coef.b1 = -(FloatType (1.0) + cosf (w0));
        coef.b2 = coef.b0;
        coef.a1 = FloatType (-2.0) * cosf (w0);
        coef.a2 = FloatType (1.0) - alpha;
        normalizeCoefficientsByA0();
    }

    /// BandPass coefficients calculation from RBJ cookbook.
    void makeBandPass (const FloatType cutOffFrequency, const FloatType Q) noexcept
    {
        const FloatType w0 = twoPi<FloatType> * cutOffFrequency / sampleRate;
        const FloatType alpha = sinf (w0) / (FloatType (2.0) * Q);

        coef.a0 = FloatType (1.0) + alpha;
        coef.b0 = alpha;
        coef.b1 = FloatType (0.0);
        coef.b2 = -alpha;
        coef.a1 = FloatType (-2.0) * cosf (w0);
        coef.a2 = FloatType (1.0) - alpha;
        normalizeCoefficientsByA0();
    }

    /// Notch coefficients calculation from RBJ cookbook.
    void makeNotch (const FloatType sampleRate, const FloatType cutOffFrequency, const FloatType Q) noexcept
    {
        const FloatType w0 = twoPi<FloatType> * cutOffFrequency / sampleRate;
        const FloatType alpha = sinf (w0) / (FloatType (2.0) * Q);

        coef.a0 = FloatType (1.0) + alpha;
        coef.b0 = FloatType (1.0);
        coef.b1 = FloatType (-2.0) * cosf (w0);
        coef.b2 = FloatType (1.0);
        coef.a1 = FloatType (-2.0) * cosf (w0);
        coef.a2 = FloatType (1.0) - alpha;
        normalizeCoefficientsByA0();
    }

    /// AllPass coefficients calculation from RBJ cookbook.
    void makeAllPass (const FloatType cutOffFrequency, const FloatType Q) noexcept
    {
        const FloatType w0 = twoPi<FloatType> * cutOffFrequency / sampleRate;
        const FloatType alpha = sinf (w0) / (FloatType (2.0) * Q);

        coef.a0 = FloatType (1.0) + alpha;
        coef.b0 = FloatType (1.0) - alpha;
        coef.b1 = FloatType (-2.0) * cosf (w0);
        coef.b2 = FloatType (1.0) + alpha;
        coef.a1 = FloatType (-2.0) * cosf (w0);
        coef.a2 = FloatType (1.0) - alpha;
        normalizeCoefficientsByA0();
    }

    /// Peak coefficients calculation from RBJ cookbook.
    void makePeakFilter (const FloatType cutOffFrequency, const FloatType Q, const FloatType gainDb) noexcept
    {
        const FloatType A = std::pow (FloatType (10.0), gainDb / FloatType (40.0));
        const FloatType w0 = twoPi<FloatType> * cutOffFrequency / sampleRate;
        const FloatType alpha = sinf (w0) / FloatType (2.0) * Q;

        coef.a0 = FloatType (1.0) + alpha / A;
        coef.b0 = FloatType (1.0) - alpha * A;
        coef.b1 = FloatType (-2.0) * cosf (w0);
        coef.b2 = FloatType (1.0) + alpha * A;
        coef.a1 = FloatType (-2.0) * cosf (w0);
        coef.a2 = FloatType (1.0) - alpha / A;
        normalizeCoefficientsByA0();
    }

    /// Low-shelf coefficients calculation from RBJ cookbook.
    void makeLowShelf (const FloatType cutOffFrequency, const FloatType Q, const FloatType gainDb) noexcept
    {
        const FloatType A = std::pow (FloatType (10.0), gainDb / FloatType (40.0));
        const FloatType w0 = twoPi<FloatType> * cutOffFrequency / sampleRate;
        const FloatType alpha = sinf (w0) / (FloatType (2.0) * Q);

        coef.a0 = FloatType (1.0) + alpha / A;
        coef.b0 = A * ((A + FloatType (1.0)) - (A - FloatType (1.0)) * cosf (w0) + FloatType (2.0) * std::sqrt (A) * alpha);
        coef.b1 = FloatType (2.0) * A * ((A - FloatType (1.0)) - (A + FloatType (1.0)) * cosf (w0));
        coef.b2 = A * ((A + FloatType (1.0)) - (A - FloatType (1.0)) * cosf (w0) - FloatType (2.0) * std::sqrt (A) * alpha);
        coef.a1 = FloatType (-2.0) * ((A - FloatType (1.0)) + (A + FloatType (1.0)) * cosf (w0));
        coef.a2 = (A + FloatType (1.0)) + (A - FloatType (1.0)) * cosf (w0) - FloatType (2.0) * std::sqrt (A) * alpha;
        normalizeCoefficientsByA0();
    }

    /// High-shelf coefficients calculation from RBJ cookbook.
    void makeHighShelf (const FloatType cutOffFrequency, const FloatType Q, const FloatType gainDb) noexcept
    {
        const FloatType A = std::pow (FloatType (10.0), gainDb / FloatType (40.0));
        const FloatType w0 = twoPi<FloatType> * cutOffFrequency / sampleRate;
        const FloatType alpha = sinf (w0) / (FloatType (2.0) * Q);

        coef.a0 = FloatType (1.0) + alpha / A;
        coef.b0 = A * ((A + FloatType (1.0)) + (A - FloatType (1.0)) * cosf (w0) + FloatType (2.0) * std::sqrt (A) * alpha);
        coef.b1 = FloatType (-2.0) * A * ((A - FloatType (1.0)) + (A + FloatType (1.0)) * cosf (w0));
        coef.b2 = A * ((A + FloatType (1.0)) + (A - FloatType (1.0)) * cosf (w0) - FloatType (2.0) * std::sqrt (A) * alpha);
        coef.a1 = FloatType (2.0) * ((A - FloatType (1.0)) - (A + FloatType (1.0)) * cosf (w0));
        coef.a2 = (A + FloatType (1.0)) - (A - FloatType (1.0)) * cosf (w0) - FloatType (2.0) * std::sqrt (A) * alpha;
        normalizeCoefficientsByA0();
    }

    /// Process audio effect.
    template <typename SampleType, size_t N>
    void process (AudioBlockView<SampleType, N>& block)
    {
        assert (block.getNumChannels() <= MaximumChannels);
        const uint_fast32_t numChannels = block.getNumChannels();
        const uint_fast32_t bufferSize = block.getNumSamplesPerChannel();

        uint_fast32_t i = 0;
        for (uint_fast32_t samp = 0; samp < bufferSize; ++samp)
        {
            for (uint_fast32_t ch = 0; ch < numChannels; ++ch)
            {
                const FloatType input = block.view[i];
                const FloatType output = coef.b0 * input + coef.b1 * x1[ch] + coef.b2 * x2[ch] - coef.a1 * y1[ch] - coef.a2 * y2[ch];
                x2[ch] = x1[ch];
                x1[ch] = input;
                y2[ch] = y1[ch];
                y1[ch] = output;
                block.view[i] = output;
                ++i;
            }
        }
    }

private:
    void normalizeCoefficientsByA0()
    {
        coef.b0 /= coef.a0;
        coef.b1 /= coef.a0;
        coef.b2 /= coef.a0;
        coef.a1 /= coef.a0;
        coef.a2 /= coef.a0;
    }

    Coefficients coef {};
    FloatType sampleRate {};
    std::array<FloatType, MaximumChannels> x1 {}; ///< [x-1] last input
    std::array<FloatType, MaximumChannels> x2 {}; ///< [x-2] second last input
    std::array<FloatType, MaximumChannels> y1 {}; ///< [y-1] last output
    std::array<FloatType, MaximumChannels> y2 {}; ///< [y-2] second last output
};
} // namespace ame::dsp