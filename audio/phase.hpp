/** 
    Phase operation.
    @file phase.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    AME is released under the MIT license.
*/

#pragma once

#include "../math/constants.hpp"

namespace ame
{
/** Increment the phase and returns in the range of 0~2pi.
    @param phase
    @param increment Amount to add to phase
    @return [0, 2pi]
*/
constexpr float addModulo2Pi (float phase, const float increment) noexcept
{
    phase += increment;
    while (phase > twoPi)
    {
        phase -= twoPi;
    }
    return phase;
}
} // namespace ame