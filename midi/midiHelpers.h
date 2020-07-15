// Copyright 2020 Akiyuki Okayasu.
//
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
//
// -------------------------------------------------------
//
// MIDI helper functions

#ifndef _NXPLIB_MIDI_HELPERS_H_
#define _NXPLIB_MIDI_HELPERS_H_

#include <cmath>

namespace nxplib
{
    class MIDItoFreq
    {
    public:
        /** Calculate MIDI note from frequency
         * Max/MSPのftomオブジェクト
         */
        static constexpr int freqToMidi(const float freq,
                                        const float A3Pitch = A3_Hz)
        {
            return std::nearbyint(12.0 * log2(freq / A3Pitch)) + A3_MIDINote;
        }

    private:
        static constexpr float A3_Hz = 440.0; // A3(MIDI note 69)の周波数
        static constexpr int A3_MIDINote = 69;

        // Disallow instantiate, this class is a holder for static methods.
        MIDItoFreq() = delete;
        // Disallow copy constructor and assignment
        MIDItoFreq(const MIDItoFreq &) = delete;
        MIDItoFreq &operator=(const MIDItoFreq &) = delete;
    };
} // namespace nxplib

#endif