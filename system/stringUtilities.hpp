/** 
    String utilities.
    @file stringUtilities.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/
#pragma once

#include <cstddef>
#include <string_view>

namespace ame
{
template <typename CharType>
constexpr bool stringComp (CharType* str, std::basic_string_view<CharType> sv)
{
    return sv.compare (str);
}
} // namespace ame