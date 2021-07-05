/** 
    
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
/** @todo comment
    @tparam length 
*/
template <int_fast32_t length>
class Wrap
{
    static_assert (length > 0, "length must be greater than 0");

public:
    Wrap() = default;
    ~Wrap() = default;

    void reset()
    {
        num = 0;
    }

    void set (const int_fast32_t n)
    {
        num = n;

        while (num >= length)
        {
            num -= length;
        }

        while (num < 0)
        {
            num += length;
        }
    }

    /**
    
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

    /**
    
    @return int_fast32_t [0, length-1]
    */
    int_fast32_t operator++() //前置インクリメント
    {
        ++num;
        if (num >= length)
        {
            num = 0;
        }
        return num;
    }

    /**
    
    @return int_fast32_t  [0, length-1]
    */
    int_fast32_t operator++ (int) //後置インクリメント
    {
        num++;
        if (num >= length)
        {
            num = 0;
        }
        return num;
    }

    /**
    
    @param add 
    @return int_fast32_t  [0, length-1]
    */
    int_fast32_t operator+= (int_fast32_t add)
    {
        num += add;
        if (num >= length)
        {
            num -= length;
        }
        return num;
    }

private:
    int_fast32_t num = 0;
};
} // namespace ame