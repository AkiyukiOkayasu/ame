// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// Math constants

#ifndef _AME_CONSTANT_H
#define _AME_CONSTANT_H

namespace ame
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
} // namespace ame

#endif // _AME_CONSTANT_H