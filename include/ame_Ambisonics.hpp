/**
    Ambisonics functions.
    @file ame_Ambisonics.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    AME is released under the MIT license.
*/

#pragma once

#include "ame_AudioBuffer.hpp"
#include "ame_Math.hpp"
#include "ame_Util.hpp"

#include <array>
#include <cassert>
#include <cstdint>

namespace ame::dsp
{
/** Polar.    
    @tparam FloatType float or double
*/
template <typename FloatType>
struct Polar
{
    FloatType azimuth {};   ///< Azimuth in radian [-π, π]
    FloatType elevation {}; ///< Elevation in radian [-π, π]
};

/** Ambisonics struct.
    @tparam FloatType float or double
*/
template <typename FloatType>
struct Ambisonics1st
{
    FloatType w {};
    FloatType y {};
    FloatType z {};
    FloatType x {};
};

/** Mono to Ambisonics 1st encode.
    @tparam FloatType float or double
    @param in Audio in
    @param out Ambisonics audio  
    @param pos Source position in radian [-π, π]
*/
template <typename FloatType>
constexpr void encodeAmbisonics1st (FloatType in, Ambisonics1st<FloatType>& out, const Polar<FloatType>& pos)
{
    out.w = in;
    out.x = in * cos (pos.azimuth) * cos (pos.elevation);
    out.y = in * sin (pos.azimuth) * cos (pos.elevation);
    out.z = in * sin (pos.elevation);
}

/** Ambisonics 1st to Mono decode.
    @tparam FloatType float or double
    @param in Ambisonics 1st audio
    @param pos Speaker position in radian [-π, π]
    @return constexpr FloatType 
*/
template <typename FloatType>
constexpr FloatType decodeAmbisonics1st (const Ambisonics1st<FloatType>& in, const Polar<FloatType>& pos)
{
    FloatType out = in.w;
    out += in.x / (cos (pos.azimuth) * cos (pos.azimuth));
    out += in.y / (sin (pos.azimuth) * cos (pos.azimuth));
    out += in.z / sin (pos.elevation);
    return out;
}

} // namespace ame::dsp
