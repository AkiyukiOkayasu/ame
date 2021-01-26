/**     
    Convert BPM and ms

    @file bpm.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

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
    N4_TRIPLET = 640,
    N4_DOT = 720,
    N4 = 480, // 4n = 480 tick
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

/** Convert BPM to ms     
    @param bpm
    @return ms ex) BPM 120 is converted to 500ms
*/
constexpr float bpmToMs (float bpm) { return 60000.0f / bpm; }

/** Convert ms to BPM
    @param ms
    @return BPM ex) 500ms is converted to BPM 120.
*/
constexpr float msToBpm (float ms) { return 60000.0f / ms; }
} // namespace ame