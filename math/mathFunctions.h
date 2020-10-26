// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// Math functions

#ifndef _AME_MATHFUNCTIONS_H
#define _AME_MATHFUNCTIONS_H

namespace ame
{
/*
Same function as the Max/MSP scale object.
*/
template <typename Type>
constexpr Type scale (Type sourceValue, Type sourceRangeMin, Type sourceRangeMax, Type targetRangeMin, Type targetRangeMax)
{    
    return targetRangeMin + ((targetRangeMax - targetRangeMin) * (sourceValue - sourceRangeMin)) / (sourceRangeMax - sourceRangeMin);
}
}

#endif // _AME_MATHFUNCTIONS_H