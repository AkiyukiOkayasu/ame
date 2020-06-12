// Copyright 2020 Akiyuki Okayasu.
// 
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)

#ifndef _NXPLIB_BUFFER_H_
#define _NXPLIB_BUFFER_H_

#include "nxplib.h"

#include <array>

namespace nxplib {
// AudioBuffer<int16_t, 2, 512> 2ch, 512samples
template<typename T, size_t channels, size_t samples>
using AudioBuffer = std::array<std::array<T, samples>, channels>;
}

#endif /* _NXPLIB_BUFFER_H_ */