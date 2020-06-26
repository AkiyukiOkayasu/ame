// Copyright 2020 Akiyuki Okayasu.
//
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
//
// -------------------------------------------------------
//
// Decibel

#ifndef _NXPLIB_DECIBEL_H_
#define _NXPLIB_DECIBEL_H_

#include "nxplib.h"
#include <cmath>
#include <algorithm>

namespace nxplib
{
    class Decibels
    {
    public:
        /** Caliculate amplitude(gain) from dB
         * Max/MSPのdbtoaオブジェクト
         */
        static inline float decibelsToGain(const float dB,
                                           const float minusInfinityDb = defaultMinusInfinitydB) noexcept
        {
            return dB > minusInfinityDb ? std::pow(10.0f, dB * 0.05f)
                                        : 0.0f;
        }

        /** Calculate dB from amplitude(gain)
        * Max/MSPのatodbオブジェクト
        */
        static inline float gainToDecibels(const float gain,
                                           const float minusInfinityDb = defaultMinusInfinitydB) noexcept
        {
            return gain > 0.0f ? std::max(minusInfinityDb, std::log10(gain) * 20.0f)
                               : minusInfinityDb;
        }

    private:
        static constexpr float defaultMinusInfinitydB = -100.0f; //-100dB以下は振幅ゼロにする

        Decibels() = delete; // This class can't be instantiated, it's just a holder for static methods..
        DISALLOW_COPY_AND_ASSIGN(Decibels);
    };

} // namespace nxplib

#endif //_NXPLIB_DECIBEL_H_
