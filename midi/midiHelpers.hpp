// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// MIDI helper functions

#pragma once

#include <cmath>

namespace ame
{
    class MIDI
    {
    public:
        /** Calculate MIDI note from frequency
         * Max/MSPのftomオブジェクト
         */
        static int freqToMidi(const float freq,
                              const float A3Freq = A3_Hz)
        {
            return static_cast<int>(std::nearbyint(12.0f * log2(freq / A3Freq))) + A3_MIDINote;
        }

        static float midiToFreq(const float midiNote,
                                const float A3Freq = A3_Hz)
        {
            return static_cast<float>(A3Freq) * std::pow(2.0f, (midiNote - A3_MIDINote) / 12.0f);
        }

    private:
        static constexpr float A3_Hz = 440.0; // A3(MIDI note 69)のデフォルト周波数
        static constexpr int A3_MIDINote = 69;

        // Disallow instantiate, this class is a holder for static methods.
        MIDI() = delete;
        // Disallow copy constructor and assignment
        MIDI(const MIDI &) = delete;
        MIDI &operator=(const MIDI &) = delete;
    };
} // namespace ame