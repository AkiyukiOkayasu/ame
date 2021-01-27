/** 
    Audio buffer.
    @file buffer.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <array>

namespace ame
{
// AudioBuffer
// 二次元配列版std::arrayの型エイリアス
// 使用例：AudioBuffer<int16_t, 2, 512> 2ch, 512samples
template <typename T, size_t channels, size_t samples>
using AudioBuffer = std::array<std::array<T, samples>, channels>;

// AudioMonoBuffer
// std::arrayの型エイリアス
// 使用例：AudioBuffer<int16_t, 512> 512samples
template <typename T, size_t samples>
using AudioMonoBuffer = AudioBuffer<T, samples, 1>;
} // namespace ame