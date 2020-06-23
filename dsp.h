
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

namespace nxplib
{
    // addModulo2Pi
    // value: value to add and modulo 2pi
    // increment:
    // return: 0~2pi
    inline float addModulo2Pi(float value, const float increment) noexcept
    {
        value += increment;
        if (value > twoPi<float>)
        {
            do
            {
                value -= twoPi<float>;
            } while (value > twoPi<float>);
        }
        return value;
    }

} // namespace nxplib

#endif //_NXPLIB_DSP_H_
