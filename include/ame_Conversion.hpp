/** 
    Audio friendly conversion functions.
    @file ame_Conversion.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "ame_Math.hpp"

#include <algorithm>
#include <cmath>
#include <concepts>

namespace ame
{
/** Enum that associates note values with MIDI ticks.
    @note 4n = 480 tick
*/
enum class NoteValue
{
    N1_DOT = 2880,
    N1 = 1920,
    N1_TRIPLET = 1280,
    N2_DOT = 1440,
    N2 = 960,
    N2_TRIPLET = 640,
    N4_DOT = 720,
    N4 = 480, ///< 4n = 480 tick
    N4_TRIPLET = 320,
    N8_DOT = 360,
    N8 = 240,
    N8_TRIPLET = 160,
    N16_DOT = 180,
    N16 = 120,
    N16_TRIPLET = 80,
    N32_DOT = 90,
    N32 = 60,
    N32_TRIPLET = 40,
    N64_DOT = 45,
    N64 = 30,
    N64_TRIPLET = 20,
    // N128_DOT cannot be expressed as an integer number of ticks
    N128 = 15,
    N128_TRIPLET = 10
};

//==============================================================================
//Constants
inline constexpr int32_t Q23_MAX = 8388607;
inline constexpr int32_t Q23_MIN = -8388608;
inline constexpr float Q23_ABSMAX = 8388608.0f;

//==============================================================================
constexpr void floatToQ23 (const float src[], int32_t dest[], const uint32_t blockSize)
{
#if defined(__GNUC__) && (__GNUC__ >= 8)
    #pragma GCC unroll 4
#endif
    for (uint32_t i = 0; i < blockSize; ++i)
    {
        dest[i] = std::clamp (static_cast<int32_t> (src[i] * Q23_MAX), Q23_MIN, Q23_MAX);
    }
}

constexpr void q23ToFloat (const int32_t src[], float dest[], const uint32_t blockSize)
{
#if defined(__GNUC__) && (__GNUC__ >= 8)
    #pragma GCC unroll 4
#endif
    for (uint32_t i = 0; i < blockSize; ++i)
    {
        dest[i] = static_cast<float> (src[i]) / Q23_ABSMAX;
    }
}

/** Split channel→Interleave conversion.
    @param source Split channel samples
    @param dest Interleave array
    @param numSamples 
    @param numChannels 
*/
constexpr void interleaveSamples (const float** source, float* dest, const uint_fast32_t numSamples, const uint_fast32_t numChannels)
{
    for (uint_fast32_t chan = 0; chan < numChannels; ++chan)
    {
        auto i = chan;
        const auto* src = source[chan];

        for (uint_fast32_t j = 0; j < numSamples; ++j)
        {
            dest[i] = src[j];
            i += numChannels;
        }
    }
}

/** Interleave→Split channel conversion.
    @param source Interleave samples
    @param dest Split channel array
    @param numSamples 
    @param numChannels 
*/
inline void deinterleaveSamples (const float* source, float** dest, const uint_fast32_t numSamples, const uint_fast32_t numChannels)
{
    for (uint_fast32_t chan = 0; chan < numChannels; ++chan)
    {
        uint_fast32_t i = chan;

        for (uint_fast32_t k = 0; k < numSamples; ++k)
        {
            dest[chan][k] = source[i];
            i += numChannels;
        }
    }
}

//==============================================================================
// Frequency

/** Convert semitones to frequency ratio.    
    @param semitone
    @return frequency ratio
    @code
    semitoneToRatio(12.0f); // 2.0f
    semitoneToRatio(-12.0f);// 0.5f
    @endcode
*/
inline float semitoneToRatio (const float semitone)
{
    return std::pow (2.0f, semitone / 12.0f);
}

/** Convert frequency to period.
    @param freq frequency in Hz
    @return period
*/
constexpr float freqToPeriod (const float freq) noexcept { return 1.0f / freq; }

/** Convert period to frequency.
    @param period period
    @return frequency
*/
constexpr float periodToFreq (const float period) noexcept { return 1.0f / period; }

/** Convert frequency to MIDI note number.
    @param frequency
    @param A3Freq Frequency for A3 (MIDI note 69). 440Hz is default.
    @return MIDI note
*/
inline float freqToMidi (const float freq, const float A3Freq = 440.0f)
{
    return 12.0f * std::log2f (freq / A3Freq) + 69.0f;
}

/** Convert MIDI note number to frequency.
    @param midiNote
    @param A3Freq Frequency for A3 (MIDI note 69). 440Hz is default.
    @return frequency in Hz
*/
inline float midiToFreq (const float midiNote, const float A3Freq = 440.0f)
{
    return A3Freq * std::pow (2.0f, (midiNote - 69.0f) / 12.0f);
}

/** 直交座標→極座標変換.    
    @param real 実部
    @param imag 虚部
    @return amplitude, angle
    @note Max/MSPのcartopolと同じ
    @see poltocar()
*/
inline std::pair<float, float> cartopol (const float real, const float imag)
{
    const float amplitude = std::sqrt (real * real + imag * imag);
    const float angle = std::atan2 (imag, real);
    return { amplitude, angle };
}

/** 極座表→直交座長変換.
    @param amplitude 
    @param angle 
    @return constexpr std::pair<float, float> 
    @note Max/MSPのpoltocarと同じ
    @see cartopol()
*/
inline std::pair<float, float> poltocar (const float amplitude, const float angle)
{
    const float real = amplitude * cosf (angle);
    const float imag = amplitude * sinf (angle);
    return { real, imag };
}

//==============================================================================
//time

/** Convert BPM to ms.    
    @param bpm
    @return ms
    @code
    bpmToMs(120.0f); // 500ms
    @endcode
*/
constexpr float bpmToMs (float bpm) { return 60000.0f / bpm; }

/** Convert ms to BPM.
    @param ms
    @return BPM
    @code
    msToBpm(500.0f); // BPM 120    
    @endcode
*/
constexpr float msToBpm (float ms) { return 60000.0f / ms; }

//==============================================================================
//volume
/** Convert a dBFS value to to its equivalent gain level.
    @param dB decibels value
    @return amplitude [0, 1]
    @note Outputs 0 if the input is less than -100dB.
    @code
    decibelsToGain (0.0f); // 1.0f
    decibelsToGain (-20.0f); // 0.1f
    @endcode
*/
constexpr float decibelsToGain (const float dB) noexcept
{
    return dB > -100.0f
               ? std::pow (10.0f, dB * 0.05f)
               : 0.0f; // Outputs 0 if the input is less than -100dB
}

/** Convert a gain level into a dBFS value.
    @param gain 
    @return dB decibels value
    @note Outputs Outputs -100dB if the input is less than 0.00001.
    @code
    gainToDecibels (1.0f); // 0dB
    gainToDecibels (0.1f); // -20.0dB
    @endcode
*/
constexpr float gainToDecibels (const float gain) noexcept
{
    return gain > 0.00001f
               ? std::log10 (gain) * 20.0f
               : -100.0f; // Minimum output value -100dB
}

//==============================================================================
/** Degree to Radians.
    @tparam FloatType float or double
    @param degree
    @return constexpr FloatType Radian
*/
template <std::floating_point FloatType>
constexpr FloatType deg2rad (FloatType degree) noexcept
{
    return degree * (ame::pi<FloatType> / static_cast<FloatType> (180.0));
}

/** Radian to Degree.
    @tparam FloatType float or double
    @param radian 
    @return constexpr FloatType degree
*/
template <std::floating_point FloatType>
constexpr FloatType rad2deg (FloatType radian) noexcept
{
    return radian * (static_cast<FloatType> (180.0) / ame::pi<FloatType>);
}
} // namespace ame