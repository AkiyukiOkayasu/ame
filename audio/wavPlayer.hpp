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
          dataChunk (reader.getDataChunk())
    {
        ///@todo 残りの実装
    }
    ~WavPlayer() = default;

    void play()
    {
        playing.store (true);
    }

    void pause()
    {
        playing.store (false);
    }

    void reset()
    {
        readPosition.store (0);
    }

    void process()
    {
        switch (formatTag)
        {
            case fmt::wFormatTag::PCM:

                break;
            case fmt::wFormatTag::ImaAdpcm:
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
    std::atomic<uint32_t> readPosition { 0 };
    std::atomic<bool> loop { false };
    std::atomic<bool> playing { false };
    const Chunk<BytePointerType> dataChunk;
};
} // namespace ame::wav