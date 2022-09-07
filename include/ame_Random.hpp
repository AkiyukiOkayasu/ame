/**
    Random
    @file ame_Random.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    AME is released under the MIT license.
*/

#pragma once

#include <bit>
#include <cstdint>

namespace
{
inline uint32_t s[4] = { 123, 234, 345, 97 };    //random seed
inline constexpr double DOUBLE_UNIT = 0x1.0p-53; // 1.0f / (1 << 53)
inline constexpr float FLOAT_UNIT = 0x1.0p-24f;  // 1.0f / (1 << 24)

inline uint32_t next (void)
{
    const uint32_t result = s[0] + s[3];

    const uint32_t t = s[1] << 9;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;

    s[3] = std::rotl (s[3], 11);

    return result;
}
} // namespace

namespace ame
{
/** Random.
    @return float [0, 1]
    @see ame::noise()
    @note Adapted from David Blackman and Sebastiano Vigna's xoshiro128+ https://prng.di.unimi.it/xoshiro128plus.c
*/
inline float random()
{
    return (next() >> 8) * FLOAT_UNIT;
}

/** White noise.
    @return float [-1, 1]
    @see ame::random()
*/
inline float noise()
{
    return (next() >> 7) * FLOAT_UNIT - 1.0f; //[-1, 1]
    /* もうひとつの方法. 負荷はほぼ同じ.
    return (static_cast<int32_t>(next()) >> 8) * 0x1.0p-23f;//[-1, 1]
    */
}
} // namespace ame
