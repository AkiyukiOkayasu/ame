/**
    Wav reader
    @file wavReader.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    AME is released under the MIT license.
*/
#pragma once

#include <cstddef>
#include <string_view>

namespace ame::wav
{
struct Chunk
{
    std::string_view id;
    uint32_t size;
    const unsigned char* data;
};

namespace fmt
{
    enum class wFormatTag : uint16_t
    {
        Unknown = 0,
        PCM = 1, //For LinearPCM
        MicrosoftAdpcm = 2,
        IeeeFloat = 3, //For 32bitFloat
        Alaw = 6,
        Ulaw = 7,
        ImaAdpcm = 0x11, // IMA-ADPCM
        YamahaAdpcm = 0x16,
        Gsm610 = 0x21,
        G721Adpcm = 0x40,
        Mpeg = 0x50,
        Experimental = 0xFF
    };
} // namespace fmt

namespace
{
    namespace wavChunkId
    {
        inline constexpr char FMT[] = "fmt ";
        inline constexpr char PEAK[] = "PEAK"; //optional
        inline constexpr char FACT[] = "fact"; //optional
        inline constexpr char DATA[] = "data";
    } // namespace wavChunkId
} // namespace

class WavReader
{
public:
    WavReader (const unsigned char* wavByteArray, size_t length)
        : wav (wavByteArray),
          length (length)
    {
        assert (length > 46); //46: RIFFヘッダーと最低限のチャンクを合わせた最小のサイズ
        parseRiffHeader();

        { //fmt chunk
            const auto fmt = parseChunk();
            assert (fmt.id == wavChunkId::FMT);
            assert (fmt.size == 16);
            formatTag = static_cast<fmt::wFormatTag> (*(reinterpret_cast<const uint16_t*> (&fmt.data[0])));
            assert (formatTag == fmt::wFormatTag::PCM || formatTag == fmt::wFormatTag::IeeeFloat || formatTag == fmt::wFormatTag::ImaAdpcm);
            numChannels = *(reinterpret_cast<const uint16_t*> (&fmt.data[2]));
            sampleRate = *(reinterpret_cast<const uint32_t*> (&fmt.data[4]));
            wBitsPerSample = *(reinterpret_cast<const uint16_t*> (&fmt.data[14]));
            assert (wBitsPerSample == 16 || wBitsPerSample == 24 || wBitsPerSample == 32);
        }

        while (true)
        {
            const auto chunk = parseChunk();
            if (chunk.id == wavChunkId::PEAK)
            {
                ///とりあえずPEAKチャンクは無視する
            }
            else if (chunk.id == wavChunkId::FACT)
            {
                //とりあえずFACTチャンクは無視する
            }
            else if (chunk.id == wavChunkId::DATA)
            {
                numSamplesPerChannel = chunk.size / numChannels / (wBitsPerSample / 8);
                dataChunk = chunk;
                break;
            }
        }
    }
    ~WavReader() = default;

    /// return RIFF header file size.
    uint32_t getFileSize() const noexcept
    {
        return fileSize;
    }

    /// return sample rate.
    uint32_t getSampleRate() const noexcept
    {
        return sampleRate;
    }

    /// return bit rate.
    uint16_t getBitRate() const noexcept
    {
        return wBitsPerSample;
    }

    /// return number of channels
    uint16_t getNumChannels() const noexcept
    {
        return numChannels;
    }

    ///return number of samples per channel.
    uint32_t getNumSamples() const noexcept
    {
        return numSamplesPerChannel;
    }

    ///return data chunk.
    Chunk getDataChunk() const noexcept
    {
        return dataChunk;
    }

    const unsigned char* getDataPointer() const noexcept
    {
        return dataChunk.data;
    }

    fmt::wFormatTag getFormatTag() const noexcept
    {
        return formatTag;
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
        const auto c = reinterpret_cast<const char*> (&wav[offset]);
        std::string_view sv (c, length - offset);
        auto chunkId = sv.substr (0, 4); //chunkIDとchunkSizeを除くチャンクサイズ
        const uint32_t chunkSize = *(reinterpret_cast<const uint32_t*> (&wav[offset + 4]));
        const size_t dataIndex = offset + 8;

        ///@todo offset更新をparseChunkから分離したい
        offset += chunkSize + 8; //8: chunkIDとchunkSizeの8byte分
        return Chunk { chunkId, chunkSize, &wav[dataIndex] };
    }

    const unsigned char* wav;
    uint32_t fileSize = 0;
    size_t length = 0;
    size_t offset = 12; //FMTチャンク冒頭

    //fmtチャンク
    fmt::wFormatTag formatTag;
    uint16_t numChannels = 0;
    uint32_t sampleRate = 0;
    uint32_t numSamplesPerChannel = 0;
    uint16_t wBitsPerSample = 0;
    Chunk dataChunk;
};
} // namespace ame::wav