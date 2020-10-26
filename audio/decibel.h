// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// Decibel
// TODO std::pow, std::max, std::log10をより高速かコンパイル時に実行できるものに置き換える

#ifndef _AME_DECIBEL_H_
#define _AME_DECIBEL_H_

#include <cmath>
#include <algorithm>

namespace ame
{
    class Decibels
    {
    public:
        /** Caliculate amplitude(gain) from dB
         * Max/MSPのdbtoaオブジェクト
         */
        static constexpr float decibelsToGain(const float dB,
                                              const float minusInfinityDb = defaultMinusInfinitydB) noexcept
        {
            return dB > minusInfinityDb ? std::pow(10.0f, dB * 0.05f)
                                        : 0.0f;
        }

        /** Calculate dB from amplitude(gain)
         * Max/MSPのatodbオブジェクト
         */
        static constexpr float gainToDecibels(const float gain,
                                              const float minusInfinityDb = defaultMinusInfinitydB) noexcept
        {
            return gain > 0.0f ? std::max(minusInfinityDb, std::log10(gain) * 20.0f)
                               : minusInfinityDb;
        }

    private:
        static constexpr float defaultMinusInfinitydB = -100.0f; //-100dB以下は振幅ゼロにする

        // Disallow instantiate, this class is a holder for static methods.
        Decibels() = delete;
        // Disallow copy constructor and assignment
        Decibels(const Decibels &) = delete;
        Decibels &operator=(const Decibels &) = delete;
    };

} // namespace ame

#endif // _AME_DECIBEL_H_