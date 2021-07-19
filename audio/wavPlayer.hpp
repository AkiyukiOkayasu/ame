/**
    Wav player
    @file wavPlayer.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    AME is released under the MIT license.
*/
#pragma once

#include "wavReader.hpp"

namespace ame::wav
{
class WavPlayer
{
public:
    WavPlayer (const WavReader reader)
        : reader (reader)
    {
        ///@todo 残りの実装
    }
    ~WavPlayer() = default;

private:
    const WavReader reader;
};
} // namespace ame::wav