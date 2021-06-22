/** 
    Filter.
    Audio-EQ-Cookbook by Robert Bristow-Johnson implementation.
    https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

    @file filter.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <atomic>

namespace ame::IIR::BiQuad //型定義
{
/** BiQuadフィルターの係数群.
    @todo atomicにするか検討
*/
struct Coefficients
{
    float a1 = 0.0f;
    float a2 = 0.0f;
    float b0 = 0.0f;
    float b1 = 0.0f;
    float b2 = 0.0f;
};

} // namespace ame::IIR::BiQuad