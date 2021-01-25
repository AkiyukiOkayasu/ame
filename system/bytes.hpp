/**
 * @file bytes.hpp
 * @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
 * @brief Byte operations
 *
 * @copyright Copyright (c) 2021 Akiyuki Okayasu
 *
 * AME is released under the MIT license.
 */

#pragma once

#include <array>
#include <cstddef>

namespace ame
{
/** Generate a std::array<std::byte, N> .
 
    std::array<std::byte, 2> b = {0x00, 0xFF}; will cause an error due to implicit casting from int to std::byte.
    std::array<std::byte, 2> b = {std::byte{0x00}, std::byte{0xFF}}; does not cause an error,
    but it is tedious, so make_bytes(0x00, 0xFF) can be used to generate it.

    @code
    // usage
    constexpr std::array<std::byte, 2> arr = make_bytes(0x00, 0xFF);
    // or
    constexpr auto arr = make_bytes(0x00, 0xFF);
    @endcode
    @tparam Ts Integer types. It must be able to be cast to std::byte.
    @param args Integers to convert to std::byte array
    @return constexpr std::array<std::byte, sizeof...(Ts)>
*/
template <typename... Ts>
constexpr std::array<std::byte, sizeof...(Ts)> make_bytes(Ts&&... args) noexcept
{
    return {std::byte(std::forward<Ts>(args))...};
}
}// namespace ame