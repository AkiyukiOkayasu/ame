/**
    PCM
    @file ame_Pcm.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    AME is released under the MIT license.
*/
#pragma once

#include "ame_AudioBuffer.hpp"
#include "ame_String.hpp"

#include <cassert>
#include <cstddef>
#include <type_traits>

///@todo PCM, Float, ADPCM用のデコーダクラス作成

namespace
{
// clang-format off
inline constexpr int16_t imaStepTable[89] = { 
  7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 
  19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 
  50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 
  130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
  337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
  876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 
  2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
  5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 
  15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767 
};
// clang-format on
} // namespace

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
    namespace wavChunkId
    {
        constexpr char fmt[] = "fmt ";
        constexpr char PEAK[] = "PEAK";
        constexpr char fact[] = "fact";
        constexpr char data[] = "data";
    } // namespace wavChunkId
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
        assert (length > 46); //46: RIFFヘッダーと最低限のチャンクを合わせた最小のサイズ
        parseRiffHeader();

        while (offset < length)
        {
            const auto chunk = parseChunk();
            switch (chunk.id)
            {
                case ChunkId::fmt:
                    //assert (chunk.size == 16);
                    formatTag = static_cast<fmt::wFormatTag> ((chunk.data[1] << 8) | chunk.data[0]);
                    assert (formatTag == fmt::wFormatTag::PCM || formatTag == fmt::wFormatTag::IeeeFloat || formatTag == fmt::wFormatTag::ImaAdpcm);
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
                    //JUNK, LIST, IDv3チャンクなどは無視する
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
        assert (wav[0] == 'R' && wav[1] == 'I' && wav[2] == 'F' && wav[3] == 'F');   //RIFF ID must be RIFF
        assert (wav[8] == 'W' && wav[9] == 'A' && wav[10] == 'V' && wav[11] == 'E'); //Format must be WAVE
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
        if (stringComp (cid, wavChunkId::fmt, 4))
        {
            chunkId = ChunkId::fmt;
        }
        else if (stringComp (cid, wavChunkId::PEAK, 4))
        {
            chunkId = ChunkId::PEAK;
        }
        else if (stringComp (cid, wavChunkId::fact, 4))
        {
            chunkId = ChunkId::fact;
        }
        else if (stringComp (cid, wavChunkId::data, 4))
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

template <typename BytePointerType>
class WavPlayer
{
    class PCMDecoder
    {
    public:
        PCMDecoder() = default;
        ~PCMDecoder() = default;

    private:
    };

public:
    explicit constexpr WavPlayer (const WavReader<BytePointerType>& reader)
        : formatTag (reader.getFormatTag()),
          numChannels (reader.getNumChannels()),
          numSamples (reader.getNumSamples()),
          bitRate (reader.getBitRate()),
          dataChunk (reader.getDataChunk())
    {
    }
    ~WavPlayer() = default;

    ///Start (or re-start) playing.
    void play()
    {
        playing.store (true);
    }

    ///Pause.
    void pause()
    {
        playing.store (false);
    }

    ///Skip to home.
    void skipToHome()
    {
        readPosition.store (0);
    }

    /** Add to sound to AudioBlockView if playing.
        @tparam FloatType float or double
        @param block dest
    */
    template <typename FloatType, size_t N>
    void process (AudioBlockView<FloatType, N>& block)
    {
        if (playing.load() == false)
        {
            return;
        }

        assert (block.getNumChannels() >= numChannels);
        assert (0 <= readPosition.load() && readPosition <= numSamples);

        ///@todo デコード部分は別関数に移動する
        switch (formatTag)
        {
            case fmt::wFormatTag::PCM:
            {
                auto offset = (bitRate / 8) * numChannels * readPosition.load();
                const auto nSamp = block.getNumSamplesPerChannel();
                for (uint_fast32_t samp = 0; samp < nSamp; ++samp)
                {
                    if (readPosition + samp >= numSamples)
                    {
                        ///再生停止 @todo 別関数をつくる
                        readPosition.store (0);
                        playing.store (false);
                        return;
                    }

                    for (uint_fast32_t ch = 0; ch < block.getNumChannels(); ++ch)
                    {
                        int32_t temp = 0;
                        for (uint32_t b = 32 - bitRate; b < 32; b += 8) //bitDepthが16bitや24bitのときは下部を0埋めする.
                        {
                            temp |= (dataChunk.data[offset] << b);
                            ++offset;
                        }
                        const FloatType s = static_cast<FloatType> (temp) / normalizeFactor; ///normalize [-1, 1]
                        block.addSample (ch, samp, s);
                    }
                }
                readPosition += nSamp;
                break;
            }
            case fmt::wFormatTag::ImaAdpcm:
                assert (false); ///@todo 実装追加
                break;
            case fmt::wFormatTag::IeeeFloat:
                assert (false); ///@todo 実装追加
                break;
            default:
                assert (false); //invalid wFormat
                break;
        }
    }

    bool isPlaying() const noexcept
    {
        return playing.load();
    }

    void setLoopPlay (bool loopPlay)
    {
        loop.store (loopPlay);
    }

    bool isLooping() const noexcept
    {
        return loop.load();
    }

private:
    const fmt::wFormatTag formatTag;
    const uint32_t numChannels;
    const uint32_t numSamples;
    const uint16_t bitRate;
    std::atomic<uint32_t> readPosition { 0 };
    std::atomic<bool> loop { false };
    std::atomic<bool> playing { false };
    const Chunk<BytePointerType> dataChunk;
    static constexpr uint32_t normalizeFactor = 2147483648; //2^31
};
} // namespace ame::wav