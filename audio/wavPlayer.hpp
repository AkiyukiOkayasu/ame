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

#include <cstddef>

///@todo PCM, Float, ADPCM用のデコーダクラス作成

namespace ame::wav
{
template <typename BytePointerType>
class WavPlayer
{
private:
    //wavデコード用内部クラス
    class WavDecoder
    {
    public:
        WavDecoder (fmt::wFormatTag formatTag, uint32_t numChannels, uint32_t numSamples)
            : formatTag (formatTag),
              numChannels (numChannels),
              numSamples (numSamples)
        {
        }
        ~WavDecoder() = default;

        float getSample (uint32_t channel, uint32_t sample)
        {
        }

    private:
        const fmt::wFormatTag formatTag = fmt::wFormatTag::Unknown;
        const uint32_t numChannels = 0;
        const uint32_t numSamples = 0;
    };

public:
    WavPlayer (const WavReader<BytePointerType> reader)
        : reader (reader)
    {
        ///@todo 残りの実装
        WavDecoder decoder (reader.getFormatTag(), reader.getNumChannels(), reader.getNumSamples()); ///@todo メンバ変数にする
    }
    ~WavPlayer() = default;

    void play();
    void pause();
    void reset();
    void process();
    bool isPlaying();
    uint32_t getNextReadPosition();
    void setNextReadPosition();
    void setLoopPlay();
    bool isLooping();

private:
    const WavReader<BytePointerType> reader;
    uint32_t readPosition = 0;
    bool loop = false;
};
} // namespace ame::wav