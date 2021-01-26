/** 
    Special functions
    @file specialFunctions.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
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
/** sinf.

    When CMSIS-DSP is available, use arm_sin_f32() of CMSIS-DSP; when not, use std::sinf()

    @param x phase
    @return float
*/
inline float sinf (float x)
{
#ifdef USE_CMSIS_DSP
    return arm_sin_f32 (x);
#else
    return std::sinf (x);
#endif
}

/** cosf.

    When CMSIS-DSP is available, use arm_cos_f32() of CMSIS-DSP; when not, use std::cosf()
    
    @param x phase
    @return float
*/
inline float cosf (float x)
{
#ifdef USE_CMSIS_DSP
    return arm_cos_f32 (x);
#else
    return std::cosf (x);
#endif
}
} // namespace ame