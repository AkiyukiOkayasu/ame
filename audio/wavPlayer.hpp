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

namespace ame::wav
{
template <typename BytePointerType>
class WavPlayer
{
public:
    constexpr WavPlayer (const WavReader<BytePointerType> reader)
        : reader (reader),
          formatTag (reader.getFormatTag()),
          numChannels (reader.getNumChannels()),
          numSamples (reader.getNumSamples()),
          dataChunk (reader.getDataChunk()),
          normalizeFactor (1 << (reader.getBitRate() - 1))
    {
        ///@todo 残りの実装
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

        assert (block.getNumChannels() == numChannels);
        assert (0 <= readPosition.load() && readPosition <= numSamples);

        auto buffer = block.getWritePointer();

        ///@todo デコード部分は別関数に移動する
        switch (formatTag)
        {
            case fmt::wFormatTag::PCM:
            {
                auto offset = (reader.getBitRate() / 8) * numChannels * readPosition.load();
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
                        for (uint32_t b = 0; b < reader.getBitRate(); b += 8)
                        {
                            temp |= (dataChunk.data[offset] << b);
                            ++offset;
                        }
                        const FloatType smp = static_cast<FloatType> (temp) / normalizeFactor; ///normalize [-1, 1]
                        buffer[samp] += smp;
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
    const WavReader<BytePointerType> reader;
    const fmt::wFormatTag formatTag;
    const uint32_t numChannels;
    const uint32_t numSamples;
    std::atomic<uint32_t> readPosition { 0 }; ///< [0, numSamples] @todo ame::Wrapに変更する？
    std::atomic<bool> loop { false };
    std::atomic<bool> playing { false };
    const Chunk<BytePointerType> dataChunk;
    const uint32_t normalizeFactor;
};
} // namespace ame::wav