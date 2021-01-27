/** 
    Interpolation.
    @file interpolation.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <atomic>

namespace ame
{
/** Linear interpolation.
    @note Function as same as std::lerp() implemented in C++20.
    @param a Start point
    @param b Target point
    @param t It expects a value of 0~1 to be entered, but specifications exceeding 1 are allowed.
    @return linear interpolated value
*/
constexpr float lerp (float a, float b, float t) noexcept { return a + t * (b - a); }
} // namespace ame