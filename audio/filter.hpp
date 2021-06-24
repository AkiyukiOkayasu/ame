/** 
    Filter.
    Audio-EQ-Cookbook by Robert Bristow-Johnson implementation.
    https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

    @file filter.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "../math/constants.hpp"
#include "buffer.hpp"

#include <array>
#include <atomic>
#include <cmath>

namespace ame::IIR::BiQuad //型定義
{
/** BiQuadフィルターの係数群.
    @todo atomicにするか検討
*/
struct Coefficients
{
    float a1 = 0.0f;
    float a2 = 0.0f;
    float b0 = 0.0f;
    float b1 = 0.0f;
    float b2 = 0.0f;
};
} // namespace ame::IIR::BiQuad

namespace ame::IIR::BiQuad //関数定義
{
/** Calculate LPF coefficients.
    @param cutOffFrequency Hz
    @param q 
    @param sampleRate 
    @return Coefficients 
*/

/// Returns the coefficients for a low-pass filter with variable Q.
inline Coefficients makeLowPass (const float sampleRate, const float cutOffFrequency, const float Q = sqrt1_2) noexcept
{
    Coefficients c;
    const float w0 = twoPi * cutOffFrequency / sampleRate;
    const float alpha = sinf (w0) / (2.0f * Q);
    const float a0 = 1.0f + alpha;

    c.b0 = (1.0f - cosf (w0)) / 2.0f;
    c.b1 = 1.0f - cosf (w0);
    c.b2 = c.b0;
    c.a1 = -2.0f * cosf (w0);
    c.a2 = 1.0f - alpha;

    c.b0 /= a0;
    c.b1 /= a0;
    c.b2 /= a0;
    c.a1 /= a0;
    c.a2 /= a0;
    return c;
}

/// Returns the coefficients for a high-pass filter with variable Q.
inline Coefficients makeHighPass (const float sampleRate, const float cutOffFrequency, const float Q = sqrt1_2) noexcept
{
    Coefficients c;
    const float w0 = twoPi * cutOffFrequency / sampleRate;
    const float alpha = sinf (w0) / (2.0f * Q);
    const float a0 = 1.0f + alpha;

    c.b0 = (1.0f + cosf (w0)) / 2.0f;
    c.b1 = -(1.0f + cosf (w0));
    c.b2 = c.b0;
    c.a1 = -2.0f * cosf (w0);
    c.a2 = 1.0f - alpha;

    c.b0 /= a0;
    c.b1 /= a0;
    c.b2 /= a0;
    c.a1 /= a0;
    c.a2 /= a0;
    return c;
}

/// Returns the coefficients for a band-pass filter with variable Q.
inline Coefficients makeBandPass (const float sampleRate, const float cutOffFrequency, const float Q = sqrt1_2) noexcept
{
    Coefficients c;
    const float w0 = twoPi * cutOffFrequency / sampleRate;
    const float alpha = sinf (w0) / (2.0f * Q);
    const float a0 = 1.0f + alpha;

    c.b0 = alpha;
    c.b1 = 0.0f;
    c.b2 = -alpha;
    c.a1 = -2.0f * cosf (w0);
    c.a2 = 1.0f - alpha;

    c.b0 /= a0;
    c.b1 /= a0;
    c.b2 /= a0;
    c.a1 /= a0;
    c.a2 /= a0;
    return c;
}

/// Returns the coefficients for a notch filter with variable Q.
inline Coefficients makeNotch (const float sampleRate, const float cutOffFrequency, const float Q = sqrt1_2) noexcept
{
    Coefficients c;
    const float w0 = twoPi * cutOffFrequency / sampleRate;
    const float alpha = sinf (w0) / (2.0f * Q);
    const float a0 = 1.0f + alpha;

    c.b0 = 1.0f;
    c.b1 = -2.0f * cosf (w0);
    c.b2 = 1.0f;
    c.a1 = -2.0f * cosf (w0);
    c.a2 = 1.0f - alpha;

    c.b0 /= a0;
    c.b1 /= a0;
    c.b2 /= a0;
    c.a1 /= a0;
    c.a2 /= a0;
    return c;
}

/// Returns the coefficients for a all-pass filter with variable Q.
inline Coefficients makeAllPass (const float sampleRate, const float cutOffFrequency, const float Q = sqrt1_2) noexcept
{
    Coefficients c;
    const float w0 = twoPi * cutOffFrequency / sampleRate;
    const float alpha = sinf (w0) / (2.0f * Q);
    const float a0 = 1.0f + alpha;

    c.b0 = 1.0f - alpha;
    c.b1 = -2.0f * cosf (w0);
    c.b2 = 1.0f + alpha;
    c.a1 = -2.0f * cosf (w0);
    c.a2 = 1.0f - alpha;

    c.b0 /= a0;
    c.b1 /= a0;
    c.b2 /= a0;
    c.a1 /= a0;
    c.a2 /= a0;
    return c;
}

/// Returns the coefficients for a peak filter with variable Q.
inline Coefficients makePeakFilter (const float sampleRate, const float cutOffFrequency, const float Q, const float gainDb) noexcept
{
    Coefficients c;
    const float A = std::pow (10.0f, gainDb / 40.0f);
    const float w0 = twoPi * cutOffFrequency / sampleRate;
    const float alpha = sinf (w0) / (2.0f * Q);
    const float a0 = 1.0f + alpha / A;

    c.b0 = 1.0f - alpha * A;
    c.b1 = -2.0f * cosf (w0);
    c.b2 = 1.0f + alpha * A;
    c.a1 = -2.0f * cosf (w0);
    c.a2 = 1.0f - alpha / A;

    c.b0 /= a0;
    c.b1 /= a0;
    c.b2 /= a0;
    c.a1 /= a0;
    c.a2 /= a0;
    return c;
}

/// Returns the coefficients for low-shelf filter with variable Q.
inline Coefficients makeLowShelf (const float sampleRate, const float cutOffFrequency, const float Q, const float gainDb) noexcept
{
    Coefficients c;
    const float A = std::pow (10.0f, gainDb / 40.0f);
    const float w0 = twoPi * cutOffFrequency / sampleRate;
    const float alpha = sinf (w0) / (2.0f * Q);
    const float a0 = 1.0f + alpha / A;

    c.b0 = A * ((A + 1.0f) - (A - 1.0f) * cosf (w0) + 2.0f * std::sqrt (A) * alpha);
    c.b1 = 2.0f * A * ((A - 1.0f) - (A + 1.0f) * cosf (w0));
    c.b2 = A * ((A + 1.0f) - (A - 1.0f) * cosf (w0) - 2.0f * std::sqrt (A) * alpha);
    c.a1 = -2.0f * ((A - 1.0f) + (A + 1.0f) * cosf (w0));
    c.a2 = (A + 1.0f) + (A - 1.0f) * cosf (w0) - 2.0f * std::sqrt (A) * alpha;

    c.b0 /= a0;
    c.b1 /= a0;
    c.b2 /= a0;
    c.a1 /= a0;
    c.a2 /= a0;
    return c;
}

/// Returns the coefficients for a high-shelf filter with variable Q.
inline Coefficients makeHighShelf (const float sampleRate, const float cutOffFrequency, const float Q, const float gainDb) noexcept
{
    Coefficients c;
    const float A = std::pow (10.0f, gainDb / 40.0f);
    const float w0 = twoPi * cutOffFrequency / sampleRate;
    const float alpha = sinf (w0) / (2.0f * Q);
    const float a0 = 1.0f + alpha / A;

    c.b0 = A * ((A + 1.0f) + (A - 1.0f) * cosf (w0) + 2.0f * std::sqrt (A) * alpha);
    c.b1 = -2.0f * A * ((A - 1.0f) + (A + 1.0f) * cosf (w0));
    c.b2 = A * ((A + 1.0f) + (A - 1.0f) * cosf (w0) - 2.0f * std::sqrt (A) * alpha);
    c.a1 = 2.0f * ((A - 1.0f) - (A + 1.0f) * cosf (w0));
    c.a2 = (A + 1.0f) - (A - 1.0f) * cosf (w0) - 2.0f * std::sqrt (A) * alpha;

    c.b0 /= a0;
    c.b1 /= a0;
    c.b2 /= a0;
    c.a1 /= a0;
    c.a2 /= a0;
    return c;
}
} // namespace ame::IIR::BiQuad

namespace ame::IIR::BiQuad //クラス定義
{
/// BiQuad filter.
template <size_t maximumChannels>
class BiQuad
{
public:
    BiQuad() = default;
    ~BiQuad() = default;

    /** Set BiQuad coefficients.
        @param c BiQuad coefficients
        @see ame::IIR::BiQuad::makeLowPass()
        @code        
        filter.setCoefficients(makeLowPass(48000.0f, 440.0f, 0.707f));
        @endcode
    */
    void setCoefficients (const Coefficients& c)
    {
        coef = c;
    }

    /// Process audio effect.
    void process (AudioBlock& block)
    {
        assert (block.getNumChannels() <= maximumChannels);

        auto buffer = block.getWritePointer();
        uint_fast32_t i = 0;
        for (uint_fast32_t samp = 0; samp < block.getNumSamples(); ++samp)
        {
            for (uint_fast32_t ch = 0; ch < block.getNumChannels(); ++ch)
            {
                const float input = buffer[i];
                const float output = coef.b0 * input + coef.b1 * x1[ch] + coef.b2 * x2[ch] - coef.a1 * y1[ch] - coef.a2 * y2[ch];
                x2[ch] = x1[ch];
                x1[ch] = input;
                y2[ch] = y1[ch];
                y1[ch] = output;
                buffer[i] = output;
                ++i;
            }
        }
    }

private:
    Coefficients coef {};
    std::array<float, maximumChannels> x1 {}; ///< [x-1] last input
    std::array<float, maximumChannels> x2 {}; ///< [x-2] second last input
    std::array<float, maximumChannels> y1 {}; ///< [y-1] last output
    std::array<float, maximumChannels> y2 {}; ///< [y-2] second last output
};
} // namespace ame::IIR::BiQuad