// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// Buffer
// TODO モノラル用のバッファー、インターリーブ用のマルチバッファーを追加

#ifndef _AME_BUFFER_H_
#define _AME_BUFFER_H_

#include <array>

// buffer
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

#endif // _AME_BUFFER_H_