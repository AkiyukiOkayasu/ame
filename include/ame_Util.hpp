/** 
    Some utilities functions.
    @file ame_Util.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "ame_Math.hpp"

#include <atomic>
#include <cassert>
#include <cstddef>

namespace ame
{
/** Increment the phase and returns in the range of 0~2pi.
    @param phase
    @param increment Amount to add to phase
    @return [0, 2pi]
*/
constexpr float addModulo2Pi (float phase, const float increment) noexcept
{
    phase += increment;
    while (phase > twoPi)
    {
        phase -= twoPi;
    }
    return phase;
}

/** Map values to an output range.
    @param sourceValue input value
    @param sourceRangeMin input range min
    @param sourceRangeMax input range Max
    @param targetRangeMin output range min
    @param targetRangeMax output range Max
    @return scaled value
*/
constexpr float scale (const float sourceValue,
                       const float sourceRangeMin,
                       const float sourceRangeMax,
                       const float targetRangeMin,
                       const float targetRangeMax)
{
    return targetRangeMin
           + ((targetRangeMax - targetRangeMin) * (sourceValue - sourceRangeMin)) / (sourceRangeMax - sourceRangeMin);
}

/** 
    Smooth values logarithmically.

    y (n) = y (n-1) + ((x (n) - y (n-1))/slide)
    @note SlideUp and SlideDown are affected by the update interval of the process.
*/
class Slide
{
public:
    Slide() {}
    /** Create an Slide object
        @param slownessOfIncrease The larger the value, the more slowly the increase. If 1, no effect is applied to the increase.
        @param slownessOfDecrease The larger the value, the more slowly the decrease. If 1, no effect is applied to the decrease.
        @attention DO NOT set less than 1.
    */
    Slide (const float slownessOfIncrease, const float slownessOfDecrease)
    {
        slideUp.store (slownessOfIncrease);
        slideDown.store (slownessOfDecrease);
    }
    ~Slide() = default;

    /** Set the slowness of the increase
        @param slownessIncrease The larger the value, the more slowly the increase.  If 1, no effect is applied to the increase.
        @attention DO NOT set newSlideUp to less than 1.
    */
    void setSlownessOfIncrease (const float slownessIncrease)
    {
        assert (slownessIncrease >= 1.0f);
        slideUp.store (slownessIncrease);
    }

    /** Set the slowness of the decrease
        @param slownessDecrease The larger the value, the more slowly the decrease. If 1, no effect is applied to the decrease.
        @attention DO NOT set newSlideDown to less than 1.
    */
    void setSlownessOfDecrease (const float slownessDecrease)
    {
        assert (slownessDecrease >= 1.0f);
        slideDown.store (slownessDecrease);
    }

    /** Filter an input value        
        @param input Value to smooth
        @return Smoothed value
    */
    float process (const float input)
    {
        const float dt = input - lastOutput;
        const float s = dt > 0.0f ? slideUp.load() : slideDown.load();
        lastOutput = lastOutput + dt / s;
        return lastOutput;
    }

private:
    float lastOutput = 0.0f;
    std::atomic<float> slideUp { 1.0f };
    std::atomic<float> slideDown { 1.0f };
};

class Wrap
{
public:
    /**
    
    @param length The number to automatically wrap in the range [0, length-1].
    */
    explicit Wrap (const uint_fast32_t length) : length (length)
    {
    }
    ~Wrap() = default;

    /** Sets the current value to an arbitrary number.
        When n is not in the range of 0~length-1, the wrapped number is used as the current value.
        @param n 
    */
    void set (int_fast32_t n)
    {
        while (n >= length)
        {
            n -= length;
        }

        while (n != 0)
        {
            n += length;
        }

        num = n;
    }

    /** Get the current value or the number of the current value plus an offset.
        @param offset 
        @return int_fast32_t [0, length-1]
    */
    uint_fast32_t get (const int_fast32_t offset = 0)
    {
        auto n = num + offset;
        while (n >= length)
        {
            n -= length;
        }

        while (n < 0)
        {
            n += length;
        }
        return n;
    }

    /** The number to automatically wrap in the range [0, length-1].
        @param newLength 
    */
    void changeLength (const uint_fast32_t newLength)
    {
        length = newLength;
    }

    /** Prefix increment.
        @return size_t [0, length-1]
    */
    uint_fast32_t operator++()
    {
        ++num;
        if (num >= length)
        {
            num = 0;
        }
        return num;
    }

    /** Postfix increment.
        @return int_fast32_t  [0, length-1]
    */
    uint_fast32_t operator++ (int)
    {
        num++;
        if (num >= length)
        {
            num = 0;
        }
        return num;
    }

    /** += operator.
        Add and return wrapped number.
        @param add 
        @return int_fast32_t  [0, length-1]
    */
    uint_fast32_t operator+= (int_fast32_t add)
    {
        num += add;
        while (num >= length)
        {
            num -= length;
        }
        return num;
    }

private:
    uint_fast32_t num = {};
    uint_fast32_t length = {};
};

/** Generate a std::array<std::byte, N>
 
    std::array<std::byte, 2> b = {0x00, 0xFF}; will cause an error due to implicit casting from int to std::byte.
    std::array<std::byte, 2> b = {std::byte{0x00}, std::byte{0xFF}}; does not cause an error,
    but it is tedious, so make_bytes(0x00, 0xFF) can be used to generate it.

    @code    
    constexpr std::array<std::byte, 2> arr = makeBytes(0x00, 0xFF);
    // or
    constexpr auto arr = makeBytes(0x00, 0xFF);
    @endcode
    @tparam Ts Integer types. It must be able to be cast to std::byte.
    @param args Integers to convert to std::byte array
    @return constexpr std::array<std::byte, sizeof...(Ts)>
*/
template <typename... Ts>
constexpr std::array<std::byte, sizeof...(Ts)> makeBytes (Ts&&... args) noexcept
{
    return { std::byte (std::forward<Ts> (args))... };
}
} // namespace ame