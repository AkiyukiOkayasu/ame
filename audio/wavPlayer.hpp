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
class WavPlayer
{
private:
    //wavデコード用内部クラス
    class WavDecoderBase
    {
    public:
        virtual float getSample (uint32_t channel, uint32_t sample) = 0;
    };

    class PCMDecoder : public WavDecoderBase
    {
    public:
        float getSample (uint32_t channel, uint32_t sample) override
        {
            return 0.0f;
        }
    };

public:
    WavPlayer (const WavReader reader)
        : reader (reader)
    {
        ///@todo 残りの実装
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
    const WavReader reader;
    uint32_t readPosition = 0;
    bool loop = false;
};
} // namespace ame::wav