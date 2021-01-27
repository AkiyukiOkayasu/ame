/** 
    Smoothing value.
    @file smoothing.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <atomic>

namespace ame
{
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
    ~Slide() {}

    /** Set the slowness of the increase
        @param newSlownessIncrease The larger the value, the more slowly the increase.  If 1, no effect is applied to the increase.
        @attention DO NOT set newSlideUp to less than 1.
    */
    void setSlownessOfIncrease (const float newSlownessIncrease)
    {
        // TODO newSlideUpが1未満の時の丸め、もしくはassert
        slideUp.store (newSlownessIncrease);
    }

    /** Set the slowness of the decrease
        @param newSlownessDecrease The larger the value, the more slowly the decrease. If 1, no effect is applied to the decrease.
        @attention DO NOT set newSlideDown to less than 1.
    */
    void setSlownessOfDecrease (const float newSlownessDecrease)
    {
        // TODO newSlideDownが1未満の時の丸め、もしくはassert
        slideDown.store (newSlownessDecrease);
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
} // namespace ame