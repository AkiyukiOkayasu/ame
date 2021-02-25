/**    
    Volume (dB, gain) related conversions.
    @file volume.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu    
    
    AME is released under the MIT license.
*/

#pragma once

#include <cmath>

namespace ame
{
/** Convert a dBFS value to to its equivalent gain level.
    @param dB decibels value
    @return amplitude [0, 1]
    @note Outputs 0 if the input is less than -100dB.
    @code
    decibelsToGain (0.0f); // 1.0f
    decibelsToGain (-20.0f); // 0.1f
    @endcode
*/
constexpr float decibelsToGain (const float dB) noexcept
{
    return dB > -100.0f
               ? std::pow (10.0f, dB * 0.05f)
               : 0.0f; // Outputs 0 if the input is less than -100dB
}

/** Convert a gain level into a dBFS value.
    @param gain 
    @return dB decibels value
    @note Outputs Outputs -100dB if the input is less than 0.00001.
    @code
    gainToDecibels (1.0f); // 0.0f
    gainToDecibels (0.1f); // -20.0f
    @endcode
*/
constexpr float gainToDecibels (const float gain) noexcept
{
    return gain > 0.00001f
               ? std::log10 (gain) * 20.0f
               : -100.0f; // Minimum output value -100dB
}
} // namespace ame