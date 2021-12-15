/** 
    Math functions
    @file ame_Math.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#if __has_include("arm_math.h")
    #ifdef __cplusplus
extern "C"
{
    #endif

    #include "arm_math.h"

    #ifdef __cplusplus
}
    #endif
    #ifndef USE_CMSIS_DSP
        #define USE_CMSIS_DSP
    #endif
#else
    #include <cmath>
    #include <type_traits>
#endif

namespace ame
{
inline constexpr float pi = 3.14159265f;      ///< π
inline constexpr float twoPi = 6.283185307f;  ///< 2π
inline constexpr float halfPi = 1.57079632f;  ///< π/2
inline constexpr float euler = 2.71828182f;   ///< Napier's constant
inline constexpr float sqrt2 = 1.41421356f;   ///< sqrt(2)
inline constexpr float sqrt1_2 = 0.70710678f; ///< sqrt(1/2)

/** sinf.

    When CMSIS-DSP is available, use arm_sin_f32() of CMSIS-DSP; when not, use std::sinf()

    @param x phase
    @return float
*/
constexpr inline float sinf (float x)
{
    if (std::is_constant_evaluated())
    {
        //コンパイル時
        auto fabs = [] (float v) -> float
        { return (v < float (0.0)) ? (-v) : (v); };

        float xSq = -(x * x);
        float series = x;
        float tmp = x;
        float fact = float (2.0);

        //マクローリン級数の計算
        do
        {
            tmp *= xSq / (fact * (fact + float (1.0)));
            series += tmp;
            fact += float (2.0);
        } while (fabs (tmp) >= std::numeric_limits<float>::epsilon());

        return series;
    }
    else
    {
        //実行時
#ifdef USE_CMSIS_DSP
        return arm_sin_f32 (x);
#else
        return std::sin (x); //libstdc++だとstd:sinf()は未定義なので、std::sin()にしている
#endif
    }
}

/** cosf.

    When CMSIS-DSP is available, use arm_cos_f32() of CMSIS-DSP; when not, use std::cosf()
    
    @param x phase
    @return float
*/
constexpr inline float cosf (float x)
{
    if (std::is_constant_evaluated())
    {
        //コンパイル時
        auto fabs = [] (float v) -> float
        { return (v < float (0.0)) ? (-v) : (v); };
        float xSq = -(x * x);
        float series = float (1.0);
        float tmp = float (1.0);
        float fact = float (1.0);

        //マクローリン級数の計算
        do
        {
            tmp *= xSq / (fact * (fact + float (1.0)));
            series += tmp;
            fact += float (2.0);
        } while (fabs (tmp) >= std::numeric_limits<float>::epsilon());

        return series;
    }
    else
    {
//実行時
#ifdef USE_CMSIS_DSP
        return arm_cos_f32 (x);
#else
        return std::cos (x); //libstdc++だとstd:cosf()は未定義なので、std::cos()にしている
#endif
    }
}

/** Linear interpolation.
    @note Function as same as std::lerp() implemented in C++20.
    @param a Start point
    @param b Target point
    @param t It expects a value of 0~1 to be entered, but specifications exceeding 1 are allowed.
    @return linear interpolated value
*/
constexpr float lerp (float a, float b, float t) noexcept
{
    return a + t * (b - a);
}
} // namespace ame