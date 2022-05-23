/** 
    Some utilities functions.
    @file ame_Util.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include "ame_Math.hpp"

#include <array>
#include <atomic>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace ame
{
/** 範囲を表す構造体.

@tparam T 
*/
template <typename T>
struct Range
{
    T min;
    T Max;
    T init;
    T step;
};

/** Increment the phase and returns in the range of 0~2pi.
    @tparam FloatType float or double
    @param phase
    @param increment Amount to add to phase
    @return [0, 2pi]
*/
template <std::floating_point FloatType>
constexpr FloatType addModulo2Pi (FloatType phase, FloatType increment) noexcept
{
    phase += increment;
    while (phase > twoPi<FloatType>)
    {
        phase -= twoPi<FloatType>;
    }
    return phase;
}

/** Map values to an output range.
    @tparam FloatType float or double
    @param sourceValue input value
    @param sourceRangeMin input range min
    @param sourceRangeMax input range Max
    @param targetRangeMin output range min
    @param targetRangeMax output range Max
    @return scaled value
*/
template <std::floating_point FloatType>
constexpr FloatType scale (FloatType sourceValue,
                           FloatType sourceRangeMin,
                           FloatType sourceRangeMax,
                           FloatType targetRangeMin,
                           FloatType targetRangeMax)
{
    return targetRangeMin
           + ((targetRangeMax - targetRangeMin) * (sourceValue - sourceRangeMin)) / (sourceRangeMax - sourceRangeMin);
}

/** Smooth values linearly.
    @tparam FloatType float or double
*/
template <std::floating_point FloatType>
class LinearSmoothedValue
{
    static_assert (! std::is_const<FloatType>::value, "FloatType is must NOT be const.");

public:
    LinearSmoothedValue (FloatType initialValue, const int rampSteps)
    {
        reset (initialValue);
        setRampLength (rampSteps);
    }
    ~LinearSmoothedValue() = default;

    /// Returns true if the current value is currently being interpolated.
    bool isSmoothing() const noexcept
    {
        return countdown > 0;
    }

    /// Returns the current value to the ramp.
    FloatType getCurrentValue() const noexcept
    {
        return currentValue;
    }

    /// Returns the target value towards which the smoothed value is currently moving.
    FloatType getTargetValue() const noexcept
    {
        return target;
    }

    /** Reset the currentValue, targetValue and ramp count.
        @param newValue The new current and target value.
    */
    void reset (FloatType newValue) noexcept
    {
        target = newValue;
        currentValue = newValue;
        countdown = 0;
    }

    /** Set a new ramp length in samples.    
        @param numSteps The number of samples to ramp.
    */
    void setRampLength (const int numSteps) noexcept
    {
        stepsToTarget = numSteps;
    }

    /** Set the next value to ramp towards.    
        @param newValue The new target value to ramp.
    */
    void setTargetValue (FloatType newValue) noexcept
    {
        if (newValue == target)
        {
            return;
        }

        if (stepsToTarget <= 0)
        {
            reset (newValue);
            return;
        }

        target = newValue;
        countdown = stepsToTarget;
        setStepSize();
    }

    /** Compute the smoothed value.
        @return FloatType Smoothed value
    */
    FloatType getNextValue() noexcept
    {
        if (! isSmoothing())
        {
            return target;
        }

        --countdown;
        if (isSmoothing())
        {
            currentValue += step;
        }
        else
        {
            currentValue = target;
        }
        return currentValue;
    }

private:
    void setStepSize()
    {
        step = (target - currentValue) / stepsToTarget;
    }

    FloatType currentValue {};
    FloatType target {};
    FloatType step {};
    int countdown {};
    int stepsToTarget {};
};

/** Smooth values logarithmically.

    y (n) = y (n-1) + ((x (n) - y (n-1))/slide)
    @note SlideUp and SlideDown are affected by the update interval of the process.
*/
template <std::floating_point FloatType>
class Slide
{
    static_assert (! std::is_const<FloatType>::value, "FloatType is must NOT be const.");

public:
    Slide() {}
    /** Create an Slide object
        @param slownessOfIncrease The larger the value, the more slowly the increase. If 1, no effect is applied to the increase.
        @param slownessOfDecrease The larger the value, the more slowly the decrease. If 1, no effect is applied to the decrease.
        @attention DO NOT set less than 1.
    */
    Slide (FloatType slownessOfIncrease, FloatType slownessOfDecrease) noexcept
    {
        slideUp.store (slownessOfIncrease);
        slideDown.store (slownessOfDecrease);
    }
    ~Slide() = default;

    /** Set the slowness of the increase
        @param slownessIncrease The larger the value, the more slowly the increase.  If 1, no effect is applied to the increase.
        @attention DO NOT set newSlideUp to less than 1.
    */
    void setSlownessOfIncrease (FloatType slownessIncrease)
    {
        assert (slownessIncrease >= 1.0f);
        slideUp.store (slownessIncrease);
    }

    /** Set the slowness of the decrease
        @param slownessDecrease The larger the value, the more slowly the decrease. If 1, no effect is applied to the decrease.
        @attention DO NOT set newSlideDown to less than 1.
    */
    void setSlownessOfDecrease (FloatType slownessDecrease)
    {
        assert (slownessDecrease >= 1.0f);
        slideDown.store (slownessDecrease);
    }

    /** Filter an input value        
        @param input Value to smooth
        @return Smoothed value
    */
    FloatType process (FloatType input) noexcept
    {
        const auto dt = input - lastOutput;
        const auto s = dt > FloatType (0.0) ? slideUp.load() : slideDown.load();
        lastOutput = lastOutput + dt / s;
        return lastOutput;
    }

private:
    FloatType lastOutput {};
    std::atomic<FloatType> slideUp { 1.0 };
    std::atomic<FloatType> slideDown { 1.0 };
};

namespace
{
    template <typename T>
    concept signed_number = std::signed_integral<T> || std::floating_point<T>;
}

/** A number to wrap between 0~length.
    @tparam T signed number (int32_t, float etc...)
*/
template <signed_number T>
class Wrap
{
public:
    /**    
    @param length The number to automatically wrap in the range [0, length-1].
    */
    explicit Wrap (T length) : length (length)
    {
        assert (length > 0); //length is must greater than 0
    }

    Wrap() = default;
    ~Wrap() = default;

    /** Sets the current value to an arbitrary number.
        When n is not in the range of 0~length-1, the wrapped number is used as the current value.
        @param n 
    */
    void set (T n)
    {
        while (n >= length)
        {
            n -= length;
        }

        while (n < 0)
        {
            n += length;
        }

        num = n;
    }

    /** Get the current value or the number of the current value plus an offset.
        @param offset 
        @return int_fast32_t [0, length-1]
    */
    T get (T offset = 0) const noexcept
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
    void changeLength (T newLength) noexcept
    {
        length = newLength;
    }

    T getLength() const noexcept
    {
        return length;
    }

    /** Prefix increment.
        @return [0, length-1]
    */
    T operator++() noexcept
    {
        ++num;
        if (num >= length)
        {
            num = 0;
        }
        return num;
    }

    /** Postfix increment.
        @return [0, length-1]
    */
    T operator++ (int) noexcept
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
        @return [0, length-1]
    */
    T operator+= (T add) noexcept
    {
        num += add;
        while (num >= length)
        {
            num -= length;
        }
        return num;
    }

private:
    T num {};
    T length {};
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