/**
 * @file semitone.hpp
 * @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
 * @brief Convert semitone and frequency ratio
 *
 * @copyright Copyright (c) 2021 Akiyuki Okayasu
 *
 * AME is released under the MIT license.
 */

#pragma once

#include <cmath>

namespace ame
{
    /**
     * @brief Convert semitones to frequency ratios. @n
     * Convert using twelve-tone equal temperament.
     *
     * @param semitone
     * @return frequency ratio @n
     * semitone 12: will return 2. @n
     * semitone -12: will return 0.5
     *
     */
    float semitoneToRatio(const float semitone)
    {
        // TODO constexpr化 std::powの置き換えが必要だが、semitoneは負数も含むので難しい
        // TODO powを高速, 軽量なものに置き換えたい
        return std::pow(2.0f, semitone / 12.0f);
    }

} // namespace ame