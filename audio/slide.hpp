/** 
    @file slide.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#include <atomic>

namespace ame
{
/** 
    Smooth values logarithmically.

    y (n) = y (n-1) + ((x (n) - y (n-1))/slide)

    Same function as the Cycling'74 Max [slide] object.
    https://docs.cycling74.com/max8/refpages/slide?q=slide

    @note SlideUp and SlideDown are affected by the update interval of the process.
*/
class Slide
{
public:
    /** Create an Slide object
        @param slownessOfIncrease The larger the value, the more slowly the increase. If 1, no effect is applied to the input.
        @param slownessOfDecrease The larger the value, the more slowly the decrease. If 1, no effect is applied to the input.
        @attention DO NOT set less than 1.
    */
    Slide(const float slownessOfIncrease, const float slownessOfDecrease)
    {
        slideUp.store(slownessOfIncrease);
        slideDown.store(slownessOfDecrease);
    }
    ~Slide() {}

    /** Set the slowness of the increase
        @param newSlideUp The larger the value, the more slowly the increase. @n When set to 1, no change is made to the increase.
        @attention DO NOT set newSlideUp to less than 1.
    */
    void setSlideUp(const float newSlideUp)
    {
        // TODO newSlideUpが1未満の時の丸め、もしくはassert
        slideUp.store(newSlideUp);
    }

    /** Set the slowness of the decrease
        @param newSlideDown The larger the value, the more slowly the decrease. @n When set to 1, no change is made to the decrease.
        @note DO NOT set newSlideDown to less than 1.
    */
    void setSlideDown(const float newSlideDown)
    {
        // TODO newSlideDownが1未満の時の丸め、もしくはassert
        slideDown.store(newSlideDown);
    }

    /** Filter an input value        
        @param input Value to smooth
        @return Smoothed value
    */
    float process(const float input)
    {
        const float dt = input - lastOutput;
        const float s = dt > 0.0 ? slideUp.load() : slideDown.load();
        lastOutput = lastOutput + dt / s;
        return lastOutput;
    }

private:
    Slide() = delete;

    float lastOutput = 0.0f;
    std::atomic<float> slideUp {1.0f};
    std::atomic<float> slideDown {1.0f};
};
}// namespace ame