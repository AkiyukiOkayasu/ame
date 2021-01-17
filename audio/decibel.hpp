/**
 * @file decibel.hpp
 * @brief Converting between decibels and amplitude
 * @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
 * Copyright (c) 2020 Akiyuki Okayasu
 * AME is released under the MIT license.
 */

// TODO std::pow, std::max, std::log10をより高速かコンパイル時に実行できるものに置き換える

#pragma once

#include <algorithm>
#include <cmath>

namespace ame
{
    /**
     * @brief Converting between decibels and amplitude
     */
    class Decibels
    {
      public:
        /**
         * @brief dB to amplitude convertion. @n This works like Cycling'74 Max's dbtoa object.
         * @param dB
         * @param minusInfinityDb The decibel value at which the amplitude is considered to be zero.
         * @return amplitude
         */
        static constexpr float decibelsToGain(const float dB,
                                              const float minusInfinityDb = defaultMinusInfinitydB) noexcept
        {
            return dB > minusInfinityDb ? std::pow(10.0f, dB * 0.05f) : 0.0f;
        }

        /**
         * @brief Amplitude to dB convertion. @n This works like Cycling'74 Max's atodb object.
         * @param gain
         * @param minusInfinityDb The decibel value at which the amplitude is considered to be zero.
         * @return dB
         * @note Cycling'74 Max / atodb object
         */
        static constexpr float gainToDecibels(const float gain,
                                              const float minusInfinityDb = defaultMinusInfinitydB) noexcept
        {
            return gain > 0.0f ? std::max(minusInfinityDb, std::log10(gain) * 20.0f) : minusInfinityDb;
        }

      private:
        /** The default decibel value, which considers the amplitude to be zero is -100 dB. */
        static constexpr float defaultMinusInfinitydB = -100.0f;

        // Disallow instantiate, this class is a holder for static methods.
        Decibels() = delete;
        // Disallow copy constructor and assignment
        Decibels(const Decibels&) = delete;
        Decibels& operator=(const Decibels&) = delete;
    };
} // namespace ame