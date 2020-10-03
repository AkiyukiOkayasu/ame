// Copyright (c) 2020 Akiyuki Okayasu
// Author: Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
// AME is released under the MIT license.
// -------------------------------------------------------
// Test
// Compile time test using static_assert()
// TODO 浮動小数点のepsilonを考慮した比較

#ifndef _AME_TEST_H_
#define _AME_TEST_H_

//#include <type_traits>
//#include <limits> std::numeric_limits
#include <tuple>

#define AME_ASSERT_EQ(value, expected) AASERT_EQ([] { return std::make_tuple(value, expected); })

template <typename X>
constexpr void AASERT_EQ(X x)
{
    static_assert(std::get<0>(x()) == std::get<1>(x()), "AASERT_EQ");
}

// Usage
/*
AME_ASSERT_EQ(0, 0);         // pass
AME_ASSERT_EQ(0, 1);         // failed
AME_ASSERT_EQ(3.14f, 3.14f); // pass
*/

#endif //_AME_TEST_H_