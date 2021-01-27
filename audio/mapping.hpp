/** 
    Mapping functions.
    @file mapping.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/
#pragma once

#include "../math/constants.hpp"

namespace ame
{
/** Map values to an output range.
    @param sourceValue input value
    @param sourceRangeMin input range min
    @param sourceRangeMax input range Max
    @param targetRangeMin output range min
    @param targetRangeMax output range Max
    @return scaled value
*/
constexpr float scale (const float sourceValue,
                       const float sourceRangeMin,
                       const float sourceRangeMax,
                       const float targetRangeMin,
                       const float targetRangeMax)
{
    return targetRangeMin
           + ((targetRangeMax - targetRangeMin) * (sourceValue - sourceRangeMin)) / (sourceRangeMax - sourceRangeMin);
}
} // namespace ame
