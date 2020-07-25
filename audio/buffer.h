// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// Buffer
// TODO モノラル用のバッファー、インターリーブ用のマルチバッファーを追加

#ifndef _AME_BUFFER_H_
#define _AME_BUFFER_H_

#include <array>
#include <algorithm>

namespace ame
{
    constexpr void float_to_q15(const float src[], int16_t dest[], uint32_t blockSize)
    {
        for (uint32_t i = 0; i < blockSize; ++i)
        {
            dest[i] = std::clamp((int16_t)(src[i] * 32768.0f), (int16_t)-32768, (int16_t)32767);
        }
    }

    constexpr void float_to_q24(const float src[], int32_t dest[], uint32_t blockSize)
    {
        for (uint32_t i = 0; i < blockSize; ++i)
        {
            dest[i] = std::clamp((int32_t)(src[i] * 8388608.0f), (int32_t)-8388608, (int32_t)8388607);
        }
    }
} // namespace ame

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