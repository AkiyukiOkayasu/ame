/** 
    @file ame_Oscillator.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "ame_Math.hpp"
#include "ame_Util.hpp"

#include <array>
#include <atomic>
#include <cassert>
#include <cmath>
#include <numeric>
#include <span>

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
constexpr std::array<FloatType, NumSamples> makeWaveTable (Function func)
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
constexpr std::array<float, N> makeSineTable()
{
    auto f = [] (auto& x)
    {
        x = ame::sin (x * ame::twoPi<float>);
    };
    return ame::makeWaveTable<float, N> (f);
}

/** Wavetable oscillator
    @tparam FloatType float or double
    @tparam numSamples 
    @todo 動作確認、単体テスト、doxygenコメントの英語化
*/
template <typename FloatType, size_t N>
class WavetableOscillator
{
    static_assert (std::is_floating_point<FloatType>::value, "FloatType is must be floating point type.");

public:
    explicit WavetableOscillator (std::span<FloatType, N> wavetable, const float sampleRate)
        : wavetable (wavetable)
    {
        tableIndex.changeLength (wavetable.size());
        setSampleRate (sampleRate);
    }
    ~WavetableOscillator() = default;

    /** Set sampling rate.   
        @param sampleRate new sampling rate in Hz
   */
    void setSampleRate (const float sampleRate)
    {
        samplingPeriod = 1.0f / sampleRate;
    }

    /** Set the frequency.
        @param freq frequency in Hz
    */
    void setFrequency (const float freq) noexcept
    {
        tableIndexIncrement = freq * tableIndex.getLength() * samplingPeriod;
    }

    /** Set the wavetable.
        @param newWavetable         
    */
    void setWavetable (std::span<FloatType, N> newWavetable)
    {
        assert (! newWavetable.empty());
        wavetable = newWavetable;
        tableIndex.changeLength (wavetable.size());
    }

    /** Generate single sample
        @return generated latest sample
    */
    FloatType nextSample() noexcept
    {
        const uint32_t aIndex = std::floor (tableIndex.get());
        const uint32_t bIndex = std::floor (tableIndex.get (1));
        const float t = tableIndex.get() - aIndex;
        const auto a = wavetable[aIndex];
        const auto b = wavetable[bIndex];

        tableIndex += tableIndexIncrement;
        return std::lerp (a, b, t);
    }

private:
    std::span<FloatType, N> wavetable;
    Wrap<float> tableIndex {};
    float samplingPeriod {};
    std::atomic<float> tableIndexIncrement {};
};

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
    SineOscillator (const float sampleRate)
    {
        setSampleRate (sampleRate);
    }
    ~SineOscillator() = default;

    /** Set sampling rate.   
        @param sampleRate new sampling rate in Hz
   */
    void setSampleRate (const float sampleRate)
    {
        samplingPeriod = 1.0f / sampleRate;
    }

    /** Set the sine wave frequency
        @param freq frequency in Hz
    */
    void setFrequency (const float freq) noexcept
    {
        phaseIncrement = freq * twoPi<float> * samplingPeriod;
    }

    /** Generate single sample
        @return generated latest sample
    */
    float nextSample() noexcept
    {
        const float p = phase.load();
        phase = addModulo2Pi (phase, phaseIncrement);
        return ame::sin (p);
    }

    /** Reset the phase to any value.    
        @param newPhase [0, 2pi]
    */
    void resetPhase (float newPhase = 0.0f)
    {
        assert (0 <= newPhase && newPhase <= twoPi<float>);
        if (0.0f <= newPhase && newPhase <= twoPi<float>)
        {
            phase = newPhase;
        }
    }

private:
    float samplingPeriod; // 1 /sampleRate
    std::atomic<float> phaseIncrement {};
    std::atomic<float> phase {}; //<[0, 2pi]

    // Disallow copy constructor and assignment
    SineOscillator (const SineOscillator&) = delete;
    SineOscillator& operator= (const SineOscillator&) = delete;
};
} // namespace ame
