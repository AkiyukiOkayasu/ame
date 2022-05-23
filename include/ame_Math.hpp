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
#endif

#include <concepts>
#include <limits>
#include <numbers>

namespace ame
{
template <std::floating_point FloatType>
inline constexpr FloatType pi = std::numbers::pi_v<FloatType>; ///<π

template <std::floating_point FloatType>
inline constexpr FloatType twoPi = 2.0 * std::numbers::pi_v<FloatType>; ///< 2π

template <std::floating_point FloatType>
inline constexpr FloatType halfPi = std::numbers::pi_v<FloatType> / 2.0; ///< π/2

template <std::floating_point FloatType>
inline constexpr FloatType sqrt2 = std::numbers::sqrt2_v<FloatType>; ///< sqrt2

template <std::floating_point FloatType>
inline constexpr FloatType invSqrt2 = 1.0 / std::numbers::sqrt2_v<FloatType>; ///< 1/sqrt2

/** sin for float

    When CMSIS-DSP is available, use arm_sin_f32() of CMSIS-DSP; when not, use std::sinf()

    @param x radian
    @return float
*/
constexpr inline float sin (float x)
{
    if (std::is_constant_evaluated())
    {
        //コンパイル時
        auto fabs = [] (float v) -> float
        { return (v < 0.0f) ? (-v) : (v); };

        float xSq = -(x * x);
        float series = x;
        float tmp = x;
        float fact = 2.0f;

        //マクローリン級数の計算
        do
        {
            tmp *= xSq / (fact * (fact + 1.0f));
            series += tmp;
            fact += 2.0f;
        } while (fabs (tmp) >= std::numeric_limits<float>::epsilon());

        return series;
    }
    else
    {
        //実行時
#ifdef USE_CMSIS_DSP
        return arm_sin_f32 (x);
#else
        return std::sin (x);
#endif
    }
}

/** sin for double.

    @param x radian
    @return double 
    @attention CMSIS-DSP does not support double, so at runtime, std::sin() is always used.
*/
constexpr inline double sin (double x)
{
    if (std::is_constant_evaluated())
    {
        //コンパイル時
        auto fabs = [] (double v) -> double
        { return (v < 0.0) ? (-v) : (v); };

        double xSq = -(x * x);
        double series = x;
        double tmp = x;
        double fact = 2.0;

        //マクローリン級数の計算
        do
        {
            tmp *= xSq / (fact * (fact + 1.0));
            series += tmp;
            fact += 2.0;
        } while (fabs (tmp) >= std::numeric_limits<double>::epsilon());

        return series;
    }
    else
    {
        //実行時
        return std::sin (x);
    }
}

/** cosf.

    When CMSIS-DSP is available, use arm_cos_f32() of CMSIS-DSP; when not, use std::cosf()
    
    @param x phase
    @return float
*/
constexpr inline float cos (float x)
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
        return std::cos (x);
#endif
    }
}

constexpr inline double cos (double x)
{
    if (std::is_constant_evaluated())
    {
        //コンパイル時
        auto fabs = [] (double v) -> double
        { return (v < 0.0) ? (-v) : (v); };
        double xSq = -(x * x);
        double series = 1.0;
        double tmp = 1.0;
        double fact = 1.0;

        //マクローリン級数の計算
        do
        {
            tmp *= xSq / (fact * (fact + 1.0));
            series += tmp;
            fact += 2.0;
        } while (fabs (tmp) >= std::numeric_limits<double>::epsilon());

        return series;
    }
    else
    {
        //実行時
        return std::cos (x); //libstdc++だとstd:cosf()は未定義なので、std::cos()にしている
    }
}
} // namespace ame