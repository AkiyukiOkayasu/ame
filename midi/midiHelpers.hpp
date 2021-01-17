/**
 * @file midiHelpers.hpp
 * @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
 * @brief MIDI helper functions
 *
 * @copyright Copyright (c) 2021 Akiyuki Okayasu
 *
 * AME is released under the MIT license.
 */

#pragma once

#include <cmath>

namespace ame
{
    class MIDI
    {
      public:
        /**
         * @brief Convert frequency to MIDI note number. @n Same function as the Cycling'74 Max [ftom] object.
         *
         * @param frequency
         * @param A3Freq Frequency for A3 (MIDI note 69). 440Hz is the most common.
         * @return MIDI note
         */
        static int freqToMidi(const float freq, const float A3Freq = A3_Hz)
        {
            return static_cast<int>(std::nearbyint(12.0f * log2(freq / A3Freq))) + A3_MIDINote;
        }

        /**
         * @brief Convert MIDI note number to frequency. @n Same function as the Cycling'74 Max [mtof] object.
         *
         * @param midiNote
         * @param A3Freq Frequency for A3 (MIDI note 69). 440Hz is the most common.
         * @return frequency in Hz
         */
        static float midiToFreq(const float midiNote, const float A3Freq = A3_Hz)
        {
            return static_cast<float>(A3Freq) * std::pow(2.0f, (midiNote - A3_MIDINote) / 12.0f);
        }

      private:
        /**
         * @brief A3 (MIDI note 69) default frequency 440Hz.
         */
        static constexpr float A3_Hz     = 440.0;
        static constexpr int A3_MIDINote = 69;

        // Disallow instantiate, this class is a holder for static methods.
        MIDI() = delete;
        // Disallow copy constructor and assignment
        MIDI(const MIDI&) = delete;
        MIDI& operator=(const MIDI&) = delete;
    };
} // namespace ame