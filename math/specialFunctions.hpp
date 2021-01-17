/**
 * @file specialFunctions.hpp
 * @brief Math special functins
 * @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
 * Copyright (c) 2020 Akiyuki Okayasu
 * AME is released under the MIT license.
 */

#pragma once

#if __has_include("arm_math.h")
#include "arm_math.h"
#ifndef USE_CMSIS_DSP
#define USE_CMSIS_DSP
#endif
#else
#include <cmath>
#endif

namespace ame
{
// sin
#ifdef USE_CMSIS_DSP
    constexpr auto sinf = arm_sin_f32();
#else
    constexpr auto sinf = std::sinf();
#endif

// cos
#ifdef USE_CMSIS_DSP
    constexpr auto cosf = arm_cos_f32();
#else
    constexpr auto cosf = std::cosf();
#endif

} // namespace ame