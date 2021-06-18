/** 
    Filter.
    @file filter.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <atomic>

namespace ame::IIR::BiQuad
{
/** BiQuadフィルターの係数群.
    @todo atomicにするか検討
*/
struct Coefficients
{
    float a0 = 0.0f;
    float a1 = 0.0f;
    float a2 = 0.0f;
    float b0 = 0.0f;
    float b1 = 0.0f;
    float b2 = 0.0f;
};

} // namespace ame::IIR::BiQuad