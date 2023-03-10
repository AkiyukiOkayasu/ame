/** 
    Analog circuit modelings
    @file ame_Circuit.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <concepts>

namespace ame
{
/** diode modeling.
    https://jatinchowdhury18.medium.com/complex-nonlinearities-epsiode-2-harmonic-exciter-cd883d888a43

    @param x 
    @return FloatType
    @note When the input is 1.0, the output is larger than 1.0    
*/
template <std::floating_point FloatType>
inline FloatType diode (FloatType x)
{
    return static_cast<FloatType> (0.2) * (std::exp (x * static_cast<FloatType> (0.05 / 0.0259)) - static_cast<FloatType> (1.0));
}
} // namespace ame