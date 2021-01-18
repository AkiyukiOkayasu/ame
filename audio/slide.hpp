/**
 * @file slide.hpp
 * @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
 * @brief Smooth values logarithmically
 *
 * @copyright Copyright (c) 2021 Akiyuki Okayasu
 *
 * AME is released under the MIT license.
 */

#include <atomic>

namespace ame
{
    /**
     * @brief Smooth values logarithmically. @n Same function as the Cycling'74 Max [slide] object.
     *
     */
    class Slide
    {
      public:
        /**
         * @brief Construct a new Slide object
         * @details The formula is y (n) = y (n-1) + ((x (n) - y (n-1))/slide).
         * @note slownessOfIncrease and slownessOfDecrease are affected by the update interval.
         *
         * @param slownessOfIncrease The larger the value, the more slowly the increase. DO NOT set less than 1.
         * @param slownessOfDecrease The larger the value, the more slowly the decrease. DO NOT set less than 1.
         */
        Slide(const float slownessOfIncrease, const float slownessOfDecrease)
        {
            slideUp.store(slownessOfIncrease);
            slideDown.store(slownessOfDecrease);
        }
        ~Slide()
        {
        }

        /**
         * @brief Set the slowness of the increase.
         *
         * @param newSlideUp The larger the value, the more slowly the increase. @n When set to 1, no change is made to
         * the increase.
         * @note DO NOT set newSlideUp to less than 1.
         */
        void setSlideUp(const float newSlideUp)
        {
            // TODO newSlideUpが1未満の時の丸め、もしくはassert
            slideUp.store(newSlideUp);
        }

        /**
         * @brief Set the slowness of the decrease.
         *
         * @param newSlideDown The larger the value, the more slowly the decrease. @n When set to 1, no change is made
         * to the decrease.
         * @note DO NOT set newSlideDown to less than 1.
         */
        void setSlideDown(const float newSlideDown)
        {
            // TODO newSlideDownが1未満の時の丸め、もしくはassert
            slideDown.store(newSlideDown);
        }

        /**
         * @brief Filter an input value.
         * @details y (n) = y (n-1) + ((x (n) - y (n-1))/slide)
         * @param input Value to smooth
         * @return Smoothed value
         */
        float process(const float input)
        {
            const float dt = input - lastOutput;
            const float s  = dt > 0.0 ? slideUp.load() : slideDown.load();
            lastOutput     = lastOutput + dt / s;
            return lastOutput;
        }

      private:
        Slide() = delete;

        float lastOutput = 0.0f;
        std::atomic<float> slideUp{1.0f};
        std::atomic<float> slideDown{1.0f};
    };
} // namespace ame