// Copyright 2020 Akiyuki Okayasu.
//
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
//
// -------------------------------------------------------
//
// Decibel

#ifndef _NXPLIB_DECIBEL_H_
#define _NXPLIB_DECIBEL_H_

#include <cmath>
#include <algorithm>

namespace nxplib
{
    constexpr float defaultMinusInfinitydB = -100.0f;

    // decibelsToGain
    // dbtoa
    // caliculate amplitude(gain) from dB
    // return
    inline float decibelsToGain(const float dB,
                                const float minusInfinityDb = defaultMinusInfinitydB) noexcept
    {
        return dB > minusInfinityDb ? std::pow(10.0f, dB * 0.05f)
                                    : 0.0f;
    }

    // gainToDecibels
    // atodb
    // calculate dB from amplitude(gain)
    inline float gainToDecibels(const float gain,
                                const float minusInfinityDb = defaultMinusInfinitydB) noexcept
    {
        return gain > 0.0f ? std::max(minusInfinityDb, std::log10(gain) * 20.0f)
                           : minusInfinityDb;
    }

} // namespace nxplib

#endif //_NXPLIB_DECIBEL_H_
