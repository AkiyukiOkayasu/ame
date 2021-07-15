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
namespace
{
    struct Chunk
    {
        char* id;
        uint32_t size;
        uint8_t* data;
    };
} // namespace
class WavReader
{
public:
    WavReader (const unsigned char* wavByteArray, size_t length) : wav (wavByteArray),
                                                                   length (length)
    {
        assert (length > 46); //46: RIFFヘッダーと最低限のチャンクを合わせた最小のサイズ
        parseRiffHeader();
    }
    ~WavReader() = default;

    /// return RIFF header file size.
    uint32_t getFileSize()
    {
        return fileSize;
    }

private:
    void parseRiffHeader()
    {
        const auto c = reinterpret_cast<const char*> (wav);
        std::string_view sv (c, length);
        auto riffId = sv.substr (0, 4);
        assert (riffId == "RIFF");
        auto format = sv.substr (8, 4);
        assert (format == "WAVE");
        fileSize = *(reinterpret_cast<const uint32_t*> (&wav[4]));
    }

    Chunk parseChunk()
    {
        char* id = nullptr;
        uint32_t size = 100;
        uint8_t* data = nullptr;
        return Chunk { id, size, data };
    }

    const unsigned char* wav;
    uint32_t fileSize = 0;
    size_t length = 0;
};
} // namespace ame