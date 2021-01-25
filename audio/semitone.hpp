/** 
    @file semitone.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <cmath>

namespace ame
{
/** Convert semitones to frequency ratios
    Convert using twelve-tone equal temperament
    @param semitone
    @return frequency ratio
    @code
    semitoneToRatio(12.0f)//  2.0f
    semitoneToRatio(-12.0f)// 0.5f
    @endcode
*/
float semitoneToRatio(const float semitone)
{
    // TODO constexpr化 std::powの置き換えが必要だが、semitoneは負数も含むので難しい
    // TODO powを高速, 軽量なものに置き換えたい
    return std::pow(2.0f, semitone / 12.0f);
}
}// namespace ame