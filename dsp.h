
// Copyright 2020 Akiyuki Okayasu.
//
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
//
// -------------------------------------------------------
//
// DSP utility functions

#ifndef _NXPLIB_DSP_H_
#define _NXPLIB_DSP_H_

#include "constant.h"
#include <limits>

namespace nxplib
{
    constexpr float fEpsilon = std::numeric_limits<float>::epsilon();

    // addModulo2Pi
    // value: value to add and modulo 2pi
    // increment:
    // return: 0~2pi
    constexpr float addModulo2Pi(float value, const float increment) noexcept
    {
        value += increment;
        while (value > twoPi<float>)
        {
            value -= twoPi<float>;
        }
        return value;
    }
    static_assert((addModulo2Pi(twoPi<float>, pi<float>) - pi<float>) <= fEpsilon,
                  "error: addModulo2Pi()");

} // namespace nxplib

#endif //_NXPLIB_DSP_H_
