// Copyright 2020 Akiyuki Okayasu.
//
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
//
// -------------------------------------------------------
//
// Buffer
// TODO モノラル用のバッファー、インターリーブ用のマルチバッファーを追加

#ifndef _NXPLIB_BUFFER_H_
#define _NXPLIB_BUFFER_H_

#include <array>

namespace nxplib
{
    // AudioBuffer
    // 二次元配列版std::arrayの型エイリアス
    // 使用例：AudioBuffer<int16_t, 2, 512> 2ch, 512samples
    template <typename T, size_t channels, size_t samples>
    using AudioBuffer = std::array<std::array<T, samples>, channels>;

} // namespace nxplib

#endif // _NXPLIB_BUFFER_H_