/** 
    String utilities.
    @file ame_String.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/
#pragma once

#include <cstdint>

namespace ame
{
template <typename Char1Type, typename Char2Type>
constexpr bool stringComp (Char1Type str1, Char2Type str2, const uint32_t size)
{
    ///@todo Char1TypeとChar2Typeにconceptを導入する
    for (uint32_t i = 0; i < size; ++i)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}
} // namespace ame