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
/** 
    Calculate LPF coefficients.
    @param cutoff Hz
    @param q 
    @param sampleRate 
    @return Coefficients 
*/
Coefficients LPFCoef (const float cutoff, const float q, const float sampleRate)
{
    Coefficients c;
    const float w0 = twoPi * cutoff / sampleRate;
    const float alpha = sinf (w0) / (2.0f * q);
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

/** 
    Calculate HPF coefficients.
    @param cutoff Hz
    @param q 
    @param sampleRate 
    @return Coefficients 
*/
Coefficients HPFCoef (const float cutoff, const float q, const float sampleRate)
{
    Coefficients c;
    const float w0 = twoPi * cutoff / sampleRate;
    const float alpha = sinf (w0) / (2.0f * q);
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

/** 
    Calculate BPF coefficients.
    Constant 0dB peak gain.
    @param cutoff Hz
    @param q 
    @param sampleRate 
    @return Coefficients 
*/
Coefficients BPFCoef (const float cutoff, const float q, const float sampleRate)
{
    Coefficients c;
    const float w0 = twoPi * cutoff / sampleRate;
    const float alpha = sinf (w0) / (2.0f * q);
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

/** 
    Calculate notch coefficients.s
    @param cutoff 
    @param q 
    @param sampleRate 
    @return Coefficients 
*/
Coefficients notchCoef (const float cutoff, const float q, const float sampleRate)
{
    Coefficients c;
    const float w0 = twoPi * cutoff / sampleRate;
    const float alpha = sinf (w0) / (2.0f * q);
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
/** 
    Calculate APF coefficients.    
    @param cutoff Hz
    @param q 
    @param sampleRate 
    @return Coefficients 
*/
Coefficients APFCoef (const float cutoff, const float q, const float sampleRate)
{
    Coefficients c;
    const float w0 = twoPi * cutoff / sampleRate;
    const float alpha = sinf (w0) / (2.0f * q);
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

/** 
    Calculate peak coefficients.    
    @param cutoff Hz
    @param q 
    @param sampleRate 
    @param dB
    @return Coefficients 
*/
Coefficients peakCoef (const float cutoff, const float q, const float sampleRate, const float dB)
{
    Coefficients c;
    const float A = std::pow (10.0f, dB / 40.0f);
    const float w0 = twoPi * cutoff / sampleRate;
    const float alpha = sinf (w0) / (2.0f * q);
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

/** 
    Calculate LowShelf coefficients.    
    @param cutoff Hz
    @param q 
    @param sampleRate 
    @param dB
    @return Coefficients 
*/
Coefficients lowShelfCoef (const float cutoff, const float q, const float sampleRate, const float dB)
{
    Coefficients c;
    const float A = std::pow (10.0f, dB / 40.0f);
    const float w0 = twoPi * cutoff / sampleRate;
    const float alpha = sinf (w0) / (2.0f * q);
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

/** 
    Calculate HighShelf coefficients.    
    @param cutoff Hz
    @param q 
    @param sampleRate 
    @param dB
    @return Coefficients 
*/
Coefficients highShelfCoef (const float cutoff, const float q, const float sampleRate, const float dB)
{
    Coefficients c;
    const float A = std::pow (10.0f, dB / 40.0f);
    const float w0 = twoPi * cutoff / sampleRate;
    const float alpha = sinf (w0) / (2.0f * q);
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