/** 
    The number to automatically wrap in the range [0, length-1].    
    @file wrap.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/
#pragma once

#include <cstddef>
#include <cstdint>

namespace ame
{
/** The number to automatically wrap in the range [0, length-1].
    @tparam length 
*/
template <int_fast32_t length>
class Wrap
{
    static_assert (length > 0, "length must be greater than 0");

public:
    Wrap() = default;
    ~Wrap() = default;

    /** Sets the current value to an arbitrary number.
        When n is not in the range of 0~length-1, the wrapped number is used as the current value.
        @param n 
    */
    void set (int_fast32_t n)
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
    int_fast32_t get (const int_fast32_t offset = 0)
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

    /** Prefix increment.
        @return size_t [0, length-1]
    */
    int_fast32_t operator++()
    {
        ++num;
        if (num >= length)
        {
            num = 0;
        }
        return num;
    }

    /** Postfix increment.
        @return int_fast32_t  [0, length-1]
    */
    int_fast32_t operator++ (int)
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
        @return int_fast32_t  [0, length-1]
    */
    int_fast32_t operator+= (int_fast32_t add)
    {
        num += add;
        while (num >= length)
        {
            num -= length;
        }
        return num;
    }

private:
    int_fast32_t num = 0;
};
} // namespace ame