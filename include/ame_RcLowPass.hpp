/** 
    RC LowPass filter.        

    @file ame_RcLowPass.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2022 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "ame_Math.hpp"

#include <cassert>
#include <type_traits>

namespace ame::dsp
{
/** RC lowpass filter for parameter smoothing.
    y[t] = x[t] * coef + (1-coef) * y[t-1]

    @tparam FloatType float or double
*/
template <typename FloatType>
class RcLowPass
{
    static_assert (std::is_floating_point<FloatType>::value, "FloatType is must be floating point type.");

public:
    explicit RcLowPass (const FloatType sampleRate)
        : samplePeriod (FloatType (1.0) / sampleRate)
    {
    }
    ~RcLowPass() = default;

    /// Set new sampleRate.
    void setSampleRate (const FloatType newSampleRate) noexcept
    {
        samplePeriod = FloatType (1.0) / newSampleRate;
    }

    /** LPF cutoff.
        a=\frac{2\pi\cdotΔT\cdot f_{c}}{2\pi\cdotΔT\cdot f_{c}+1}
        @param cutOffFrequency cutoff frequency in Hz        
    */
    void setCutOffFrequency (const FloatType cutOffFrequency) noexcept
    {
        coef = (ame::twoPi<FloatType> * cutOffFrequency * samplePeriod) / (ame::twoPi<FloatType> * cutOffFrequency * samplePeriod + 1);
    }

    /** Set raw coefficient value.
        @param newRawCoefficient [0.0 < newRawCoefficient <= 1.0]
        @attention 0より大きな、1以下の値でないといけない。0だとy[t-1]のみがバイアスとして出力されてしまうし、1以上だと発散してしまう。
    */
    void setRawCoefficient (const FloatType newRawCoefficient)
    {
        assert (0.0f < newRawCoefficient && newRawCoefficient <= 1.0f);
        coef = newRawCoefficient;
    }

    /** Get raw value of coefficient.
        @return FloatType 
    */
    FloatType getRawCoefficient() const noexcept
    {
        return coef;
    }

    /** Process filter.
    @param input 
    @return FloatType Filtered output
    */
    FloatType process (FloatType input) noexcept
    {
        lastOutput = coef * input + (FloatType (1.0) - coef) * lastOutput;
        return lastOutput;
    }

private:
    FloatType coef { 1.0 };
    FloatType lastOutput {};
    FloatType samplePeriod { 1.0f / 48000.0f };
};
} // namespace ame::dsp