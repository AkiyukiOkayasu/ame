/**
 * @file bytes.hpp
 * @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
 * @brief
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
    /**
     * @brief Generate a std::array<std::byte, N> @n
     * std::array<std::byte, 4> b = {0, 1, 2, 3}; will cause an error due to implicit casting from int to std::byte. @n
     * std::array<std::byte, 4> b = {std::byte{0}, std::byte{1}, std::byte{2}, std::byte{3}}; does not cause an error,
     * but it is tedious, so make_bytes(0, 1, 2, 3) can be used to generate it.
     * @code
     * // usage
     * constexpr std::array<std::byte, 4> arr = make_bytes(0, 1, 2, 3);
     * // or
     * constexpr auto arr = make_bytes(0, 1, 2, 3);
     * @endcode
     * @tparam Ts
     * @param args
     * @return constexpr std::array<std::byte, sizeof...(Ts)>
     */
    template<typename... Ts> constexpr std::array<std::byte, sizeof...(Ts)> make_bytes(Ts&&... args) noexcept
    {
        return {std::byte(std::forward<Ts>(args))...};
    }
} // namespace ame