/**
    Wav player
    @file wavPlayer.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu

    AME is released under the MIT license.
*/
#pragma once

#include "audioBlockView.hpp"
#include "wavReader.hpp"

#include <atomic>
#include <cassert>
#include <cstddef>

///@todo PCM, Float, ADPCM用のデコーダクラス作成

namespace
{
// clang-format off
inline constexpr int16_t ima_step_table[89] = { 
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
    explicit constexpr WavPlayer (const WavReader<BytePointerType> reader)
        : formatTag (reader.getFormatTag()),
          numChannels (reader.getNumChannels()),
          numSamples (reader.getNumSamples()),
          dataChunk (reader.getDataChunk()),
          bitRate (reader.getBitRate())
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
    template <typename FloatType>
    void process (AudioBlockView<FloatType>& block)
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
                for (uint_fast32_t samp = 0; samp < block.getNumSamples(); ++samp)
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
                readPosition += block.getNumSamples();
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
    std::atomic<uint32_t> readPosition { 0 }; ///< [0, numSamples] @todo ame::Wrapに変更する？
    std::atomic<bool> loop { false };
    std::atomic<bool> playing { false };
    const Chunk<BytePointerType> dataChunk;
    static constexpr uint32_t normalizeFactor = 2147483648; //2^31
};
} // namespace ame::wav