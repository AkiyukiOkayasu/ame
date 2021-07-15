/**
    Wav reader
    @file wavReader.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    AME is released under the MIT license.
*/
#pragma once

#include <cstddef>
#include <iostream>
#include <string_view>

namespace ame
{
class WavReader
{
public:
    WavReader (const uint8_t* wavByteArray, size_t length) : wav (wavByteArray),
                                                             length (length)
    {
        //46: チャンク全体の最小サイズ1
        assert (length > 46);

        //RIFFチャンク
        {
            std::cout << "wav: " << wav[0] << std::endl;
            const auto c = reinterpret_cast<const char*> (wav);
            std::cout << "wav_c: " << c[0] << std::endl;
            std::string_view sv (c);
            std::cout << "sv: " << sv[0] << std::endl;
            auto subsv = sv.substr (0, 4);
            assert (subsv == "RIFF");
            //std::cout << sv[0] << std::endl;
            //assert (sv[0] == 'R');
        }
    }
    ~WavReader() = default;

private:
    const uint8_t* wav;
    [[maybe_unused]] size_t length = 0;
};
} // namespace ame