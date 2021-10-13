/** 
    @file ame_Oscillator.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "ame_Math.hpp"
#include "ame_Util.hpp"

#include <atomic>

namespace ame
{
/** Wavetable generator.
    @tparam FloatType float or double
    @tparam NumSamples array size
    @tparam Function function to generate wavetable
    @note Functionの入力は0~1範囲の配列です    
    @see make_sineTable()
*/
template <typename FloatType, size_t NumSamples, class Function>
constexpr auto makeWaveTable (Function func)
{
    std::array<FloatType, NumSamples> ar;
    std::iota (ar.begin(), ar.end(), 0);
    std::for_each (ar.begin(), ar.end(), [] (auto& x)
                   { x = x / (NumSamples - 1); }); // ar: 0~1が順に並んだ配列
    std::for_each (ar.begin(), ar.end(), func);
    return ar;
}

/** Sine wave wavetable generator.
    @tparam N array size
    @return constexpr std::array<float, N> sine wave table
    @todo 初期位相いじれるようにする？
*/
template <size_t N>
constexpr auto makeSineTable()
{
    auto f = [] (auto& x)
    {
        x = ame::sinf (x * ame::twoPi);
    };
    return ame::makeWaveTable<float, N> (f);
}

/** Sine wave oscillator.
    Generates a sine between -1.0~1.0.
*/
class SineOscillator
{
public:
    /** Create sine wave oscillator instance.
        @param sampleRate The sample rate that will be used for calclate the oscillator phase  increment.
        @param frequency Initial frequency
    */
    SineOscillator (const float sampleRate, const float frequency) : samplingPeriod (1.0f / sampleRate)
    {
        setFrequency (frequency);
    }
    ~SineOscillator() {}

    /** Set the sine wave frequency
        @param freq frequency in Hz
    */
    void setFrequency (const float freq) noexcept { phaseIncrement = freq * twoPi * samplingPeriod; }

    /** Generate single sample
        @return generated latest sample
    */
    float nextSample() noexcept
    {
        phase = addModulo2Pi (phase, phaseIncrement);
        return ame::sinf (phase);
    }

private:
    float samplingPeriod; // サンプリング周期 1 /sampleRate
    std::atomic<float> phaseIncrement { 0.0f };
    std::atomic<float> phase { 0.0f };

    // Disallow copy constructor and assignment
    SineOscillator (const SineOscillator&) = delete;
    SineOscillator& operator= (const SineOscillator&) = delete;
};
} // namespace ame
