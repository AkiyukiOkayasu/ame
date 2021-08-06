/**
    Wav reader
    @file wavReader.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    AME is released under the MIT license.
*/
#pragma once

#include "../system/stringUtilities.hpp"

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace ame::wav
{
enum class ChunkId : uint_fast8_t
{
    Unknown = 0,
    fmt,
    PEAK,
    fact,
    data
};

template <class BytePointerType>
struct Chunk
{
    constexpr Chunk() = default;
    ChunkId id;
    uint32_t size;
    BytePointerType data;
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
    namespace WavChunkId
    {
        constexpr char fmt[] = "fmt ";
        constexpr char PEAK[] = "PEAK";
        constexpr char fact[] = "fact";
        constexpr char data[] = "data";
    } // namespace WavChunkId
} // namespace

template <typename BytePointerType>
class WavReader
{
private:
    using baseType = typename std::remove_pointer<BytePointerType>::type;
    using cvRemovedBaseType = typename std::remove_cv<baseType>::type;
    static_assert (std::is_pointer<BytePointerType>::value, "BytePointerType must be a Pointer type.");
    static_assert (std::is_same<cvRemovedBaseType,
                                unsigned char>::value,
                   "BytePointerType must be unsigned char* (or CV-qualified).");

public:
    constexpr WavReader (BytePointerType wavByteArray, size_t length)
        : wav (wavByteArray),
          length (length)
    {
        //assert (length > 46); //46: RIFFヘッダーと最低限のチャンクを合わせた最小のサイズ
        parseRiffHeader();

        while (offset < length)
        {
            const auto chunk = parseChunk();
            switch (chunk.id)
            {
                case ChunkId::fmt:
                    //assert (chunk.size == 16);
                    formatTag = static_cast<fmt::wFormatTag> ((chunk.data[1] << 8) | chunk.data[0]);
                    //assert (formatTag == fmt::wFormatTag::PCM || formatTag == fmt::wFormatTag::IeeeFloat || formatTag == fmt::wFormatTag::ImaAdpcm);
                    numChannels = (chunk.data[3] << 8) | chunk.data[2];
                    sampleRate = (chunk.data[5] << 8) | chunk.data[4];
                    wBitsPerSample = (chunk.data[15] << 8) | chunk.data[14];
                    //assert (wBitsPerSample == 16 || wBitsPerSample == 24 || wBitsPerSample == 32);
                    break;
                case ChunkId::PEAK:
                    //とりあえずPEAKチャンクは無視する
                    break;
                case ChunkId::fact:
                    //とりえあずfactチャンクは無視する
                    break;
                case ChunkId::data:
                    numSamplesPerChannel = chunk.size / numChannels / (wBitsPerSample / 8);
                    dataChunk = chunk;
                    break;
                default:
                    //assert (false); //invalid chunk id
                    break;
            }
        }
    }
    ~WavReader() = default;

    /// return RIFF header file size.
    constexpr uint32_t getFileSize() const noexcept
    {
        return fileSize;
    }

    /// return sample rate.
    constexpr uint32_t getSampleRate() const noexcept
    {
        return sampleRate;
    }

    /// return bit rate.
    constexpr uint16_t getBitRate() const noexcept
    {
        return wBitsPerSample;
    }

    /// return number of channels
    constexpr uint16_t getNumChannels() const noexcept
    {
        return numChannels;
    }

    ///return number of samples per channel.
    constexpr uint32_t getNumSamples() const noexcept
    {
        return numSamplesPerChannel;
    }

    ///return data chunk.
    constexpr Chunk<BytePointerType> getDataChunk() const noexcept
    {
        return dataChunk;
    }

    constexpr BytePointerType getDataPointer() const noexcept
    {
        return dataChunk.data;
    }

    constexpr fmt::wFormatTag getFormatTag() const noexcept
    {
        return formatTag;
    }

private:
    constexpr void parseRiffHeader()
    {
        //assert (wav[0] == 'R' && wav[1] == 'I' && wav[2] == 'F' && wav[3] == 'F');   //RIFF ID must be RIFF
        //assert (wav[8] == 'W' && wav[9] == 'A' && wav[10] == 'V' && wav[11] == 'E'); //Format must be WAVE
        fileSize = (wav[7] << 24) | (wav[6] << 16) | (wav[5] << 8) | wav[4];
    }

    constexpr Chunk<BytePointerType> parseChunk()
    {
        unsigned char cid[4] = {};
        for (int i = 0; i < 4; ++i)
        {
            cid[i] = wav[offset + i];
        }

        ChunkId chunkId = ChunkId::Unknown;
        if (stringComp (cid, WavChunkId::fmt, 4))
        {
            chunkId = ChunkId::fmt;
        }
        else if (stringComp (cid, WavChunkId::PEAK, 4))
        {
            chunkId = ChunkId::PEAK;
        }
        else if (stringComp (cid, WavChunkId::fact, 4))
        {
            chunkId = ChunkId::fact;
        }
        else if (stringComp (cid, WavChunkId::data, 4))
        {
            chunkId = ChunkId::data;
        }

        const uint32_t chunkSize = (wav[offset + 7] << 24) | (wav[offset + 6] << 16) | (wav[offset + 5] << 8) | wav[offset + 4]; //constexprのためにretinterpret_cast<uint32_t>を使わない
        const size_t dataIndex = offset + 8;

        ///@todo offset更新をparseChunkから分離したい
        offset += chunkSize + 8; //8: chunkIDとchunkSizeの8byte分
        return Chunk<BytePointerType> { chunkId, chunkSize, &wav[dataIndex] };
    }

    const BytePointerType wav;
    uint32_t fileSize = 0;
    const size_t length = 0;
    size_t offset = 12; //FMTチャンク冒頭

    //fmtチャンク
    fmt::wFormatTag formatTag = fmt::wFormatTag::Unknown;
    uint16_t numChannels = 0;
    uint32_t sampleRate = 0;
    uint32_t numSamplesPerChannel = 0;
    uint16_t wBitsPerSample = 0;
    Chunk<BytePointerType> dataChunk {};
};
} // namespace ame::wav