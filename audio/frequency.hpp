/** 
    Frequency related conversions.
    
    @file freqquency.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <cmath>

namespace ame
{
/** Convert semitones to frequency ratio.    
    @param semitone
    @return frequency ratio
    @code
    semitoneToRatio(12.0f)//  2.0f
    semitoneToRatio(-12.0f)// 0.5f
    @endcode
*/
float semitoneToRatio (const float semitone)
{
    // TODO constexpr化 std::powの置き換えが必要だが、semitoneは負数も含むので難しい
    // TODO powを高速, 軽量なものに置き換えたい
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
static float freqToMidi (const float freq, const float A3Freq = 440.0f)
{
    return 12.0f * log2 (freq / A3Freq) + 69.0f;
}

/** Convert MIDI note number to frequency.
        @param midiNote
        @param A3Freq Frequency for A3 (MIDI note 69). 440Hz is default.
        @return frequency in Hz
*/
static float midiToFreq (const float midiNote, const float A3Freq = 440.0f)
{
    return A3Freq * std::pow (2.0f, (midiNote - 69.0f) / 12.0f);
}
} // namespace ame