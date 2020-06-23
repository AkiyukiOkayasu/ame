// Copyright 2020 Akiyuki Okayasu.
//
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)

#ifndef _NXPLIB_CONSTANT_H
#define _NXPLIB_CONSTANT_H

#include "nxplib.h"
#include "math.h"

namespace nxplib
{
    template <typename T>
    constexpr T pi = static_cast<T>(3.141592653589793238L);

    template <typename T>
    constexpr T twoPi = static_cast<T>(2.0L * 3.141592653589793238L);

    template <typename T>
    constexpr T halfPi = static_cast<T>(3.141592653589793238L / 2.0L);

    template <typename T>
    constexpr T euler = static_cast<T>(2.71828182845904523536L);

    template <typename T>
    constexpr T sqrt2 = static_cast<T>(1.4142135623730950488L);
} // namespace nxplib

#endif // _NXPLIB_CONSTANT_H