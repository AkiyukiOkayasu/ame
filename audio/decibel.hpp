/**
 * @file decibel.hpp
 * @brief Audio buffer
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
     * @brief dB - amplitude convertion
     *
     */
    class Decibels
    {
      public:
        /**
         * @brief dB to amplitude
         *
         * @param dB
         * @param minusInfinityDb
         * @return constexpr float amplitude
         */
        static constexpr float decibelsToGain(const float dB,
                                              const float minusInfinityDb = defaultMinusInfinitydB) noexcept
        {
            return dB > minusInfinityDb ? std::pow(10.0f, dB * 0.05f) : 0.0f;
        }

        /** Calculate dB from amplitude(gain)
         * Max/MSPのatodbオブジェクト
         */

        /**
         * @brief amplitude to dB
         *
         * @param gain
         * @param minusInfinityDb
         * @return constexpr float dB
         */
        static constexpr float gainToDecibels(const float gain,
                                              const float minusInfinityDb = defaultMinusInfinitydB) noexcept
        {
            return gain > 0.0f ? std::max(minusInfinityDb, std::log10(gain) * 20.0f) : minusInfinityDb;
        }

      private:
        static constexpr float defaultMinusInfinitydB = -100.0f; //-100dB以下は振幅ゼロにする

        // Disallow instantiate, this class is a holder for static methods.
        Decibels() = delete;
        // Disallow copy constructor and assignment
        Decibels(const Decibels&) = delete;
        Decibels& operator=(const Decibels&) = delete;
    };
} // namespace ame