// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// MIDI helper functions

#ifndef _AME_MIDI_HELPERS_H_
#define _AME_MIDI_HELPERS_H_

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
            return std::nearbyintf(12.0 * log2(freq / A3Freq)) + A3_MIDINote;
        }

        static int midiToFreq(const float midiNote,
                              const float A3Freq = A3_Hz)
        {
            return A3Freq * std::powf(2.0f, (midiNote - A3_MIDINote) / 12.0f);
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

#endif // _AME_MIDI_HELPERS_H_