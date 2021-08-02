#define CATCH_CONFIG_MAIN
#include "ame.hpp"
#include "resource/sine440.hpp"

#include <catch2/catch.hpp>
#include <iomanip>

TEST_CASE ("Volume")
{
    SECTION ("decibelsToGain()")
    {
        REQUIRE (ame::decibelsToGain (0.0f) == Approx (1.0f));
        REQUIRE (ame::decibelsToGain (-6.0f) == Approx (0.501187));
        REQUIRE (ame::decibelsToGain (-20.0f) == Approx (0.1));
        REQUIRE (ame::decibelsToGain (3.0f) == Approx (1.412538));
    }
    SECTION ("gainToDecibels()")
    {
        REQUIRE (ame::gainToDecibels (1.0f) == Approx (0.0f));
        REQUIRE (ame::gainToDecibels (0.501187f) == Approx (-6.0f));
        REQUIRE (ame::gainToDecibels (0.1f) == Approx (-20.0f));
        REQUIRE (ame::gainToDecibels (1.412538f) == Approx (3.0f));
    }
}

TEST_CASE ("Time")
{
    SECTION ("addModule2Pi()")
    {
        REQUIRE (ame::addModulo2Pi (6.28318530717958647692f, 0.1f) == Approx (0.1f));
        REQUIRE (ame::addModulo2Pi (6.28318530717958647692f, 2.5f) == Approx (2.5f));
    }

    SECTION ("freqToPeriod()")
    {
        REQUIRE (ame::freqToPeriod (440.0f) == Approx (0.002272727f));
        REQUIRE (ame::freqToPeriod (44100.0f) == Approx (0.000022676f));
    }

    SECTION ("periodToFreq()")
    {
        REQUIRE (ame::periodToFreq (0.002272727f) == Approx (440.0f));
        REQUIRE (ame::periodToFreq (0.000022676f) == Approx (44100.0f));
    }

    SECTION ("scale()")
    {
        REQUIRE (ame::scale (0.5f, 0.0f, 1.0f, -100.0f, 100.0f) == Approx (0.0f));
        REQUIRE (ame::scale (0.0f, 0.0f, 1.0f, -100.0f, 100.0f) == Approx (-100.0f));
    }

    SECTION ("BPM to ms")
    {
        REQUIRE (ame::bpmToMs (60.0f) == Approx (1000.0f));
        REQUIRE (ame::bpmToMs (120.0f) == Approx (500.0f));
    }

    SECTION ("ms to BPM")
    {
        REQUIRE (ame::msToBpm (1000.0f) == Approx (60.0f));
        REQUIRE (ame::msToBpm (500.0f) == Approx (120.0f));
    }
}

TEST_CASE ("Frequency")
{
    SECTION ("Convert pol between car")
    {
        auto [amp, angle] = ame::cartopol (0.9, -0.4);
        REQUIRE (amp == Approx (0.984886f));
        REQUIRE (angle == Approx (-0.418224f));
        auto [real, imag] = ame::poltocar (amp, angle);
        REQUIRE (real == Approx (0.9));
        REQUIRE (imag == Approx (-0.4));
    }
}

TEST_CASE ("MIDI")
{
    SECTION ("ftom()") { REQUIRE (ame::freqToMidi (440.0f) == Approx (69)); }
    SECTION ("mtof()") { REQUIRE (ame::midiToFreq (69) == Approx (440.0f)); }
}

TEST_CASE ("WaveTable")
{
    SECTION ("makeTable()")
    {
        auto f = [] (auto& x)
        {
            x = ame::sinf (x * ame::twoPi);
        };

        auto ar = ame::makeWaveTable<float, 5> (f);

        /* 
        -0.0f == Approx(0.0f)を失敗させないために、Approx().scale(1)を指定する
        https://github.com/catchorg/Catch2/issues/1079
        */
        REQUIRE (ar[0] == Approx (0.0f).scale (1));
        REQUIRE (ar[1] == Approx (1.0f).scale (1));
        REQUIRE (ar[2] == Approx (0.0f).scale (1));
        REQUIRE (ar[3] == Approx (-1.0f).scale (1));
        REQUIRE (ar[4] == Approx (0.0f).scale (1));
    }
}

TEST_CASE ("Filter")
{
    SECTION ("Biquad")
    {
        constexpr int numChannels = 2;
        constexpr int numSamples = 1000;
        std::array<float, numChannels * numSamples> buffer {};
        ame::AudioBlockView block (buffer.data(), numChannels, numSamples); //Stereo
        ame::dsp::iir::biquad::BiQuad<numChannels> filter;

        // LPF
        filter.setCoefficients (ame::dsp::iir::biquad::makeLowPass (44100.0f, 100.0f, 0.71f));
        filter.process (block);
        for (int i = 0; i < numSamples * numChannels; ++i)
        {
            REQUIRE (buffer[i] == Approx (0.0f));
        }

        // HPF for remove DC offset;
        filter.setCoefficients (ame::dsp::iir::biquad::makeHighPass (44100.0f, 200.0f, 0.71f));
        buffer.fill (0.3f); //DC offset;
        filter.process (block);
        for (int i = 500; i < numSamples * numChannels; ++i)
        {
            REQUIRE (buffer[i] <= 0.01f);
        }
    }
}

TEST_CASE ("Wrap")
{
    SECTION ("Increment")
    {
        ame::Wrap<10> w;
        REQUIRE (w.get() == 0);
        REQUIRE (w++ == 1);
        REQUIRE (++w == 2);
        w += 2;
        REQUIRE (w.get() == 4);
        w += 9;
        REQUIRE (w.get() == 3);
    }

    SECTION ("set")
    {
        ame::Wrap<10> w;
        w.set (12);
        REQUIRE (w.get() == 2);
        w.set (-9);
        REQUIRE (w.get() == 1);
    }
}

TEST_CASE ("AudioBuffer")
{
    SECTION ("getPeak")
    {
        ame::AudioBuffer<float, 4> buf (2);
        auto b = buf.getWritePointer();
        b[0] = 0.0f;
        b[1] = 0.2f;
        b[2] = 0.9f;
        b[3] = -0.5f;
        buf.getPeak (0);
        REQUIRE (buf.getPeak (0) == Approx (0.9f));
        REQUIRE (buf.getPeak (1) == Approx (0.5f));
    }
}

TEST_CASE ("AudioBlockView")
{
    SECTION ("getPeak")
    {
        float v[6] = { 0.0f, 0.2f, 0.9f, -0.5f, -0.1f, -0.0f };
        ame::AudioBlockView<float> block (v, 2, 3);
        REQUIRE (block.getPeak (0) == Approx (0.9f));
        REQUIRE (block.getPeak (1) == Approx (0.5f));
    }
}

TEST_CASE ("WavReader")
{
    constexpr ame::wav::WavReader wavReader (wav::wav, sizeof (wav::wav));
    SECTION ("getFileSize()")
    {
        REQUIRE (wavReader.getFileSize() == sizeof (wav::wav) - 8);
    }

    SECTION ("sampleRate")
    {
        REQUIRE (wavReader.getSampleRate() == 44100);
    }

    SECTION ("bitRate")
    {
        REQUIRE (wavReader.getBitRate() == 16);
    }

    SECTION ("Channel")
    {
        REQUIRE (wavReader.getNumChannels() == 1);
    }

    SECTION ("numSample")
    {
        REQUIRE (wavReader.getNumSamples() == 88200);
    }

    SECTION ("getDataPointer")
    {
        auto data = wavReader.getDataPointer();
        REQUIRE (data[0] == 0x00);
        REQUIRE (data[1] == 0x00);
        REQUIRE (data[2] == 0x6B);
        REQUIRE (data[3] == 0x06);
        REQUIRE (data[4] == 0xCC);
        REQUIRE (data[5] == 0x0C);
        REQUIRE (data[6] == 0x28);
        REQUIRE (data[7] == 0x13);
    }
}

TEST_CASE ("WavPlayer")
{
    SECTION ("Constructor")
    {
        constexpr ame::wav::WavReader reader (wav::wav, sizeof (wav::wav));
        constexpr ame::wav::WavPlayer player (reader);
    }
}

TEST_CASE ("Byte")
{
    SECTION ("makeByte")
    {
    }
}

TEST_CASE ("String")
{
    SECTION ("Comp")
    {
        char foo[] = { 'f', 'o', 'o' };
        const char bar[] = { 'b', 'a', 'r' };
        char baz[] = "baz";
        unsigned char qux[] = "qux";
        unsigned char quux[] = "quux";
        REQUIRE (ame::stringComp (foo, "foo", 3));
        REQUIRE (ame::stringComp (bar, "bar", 3));
        REQUIRE (ame::stringComp (baz, "baz", 3));
        REQUIRE (ame::stringComp (qux, "qux", 3));
        REQUIRE (! ame::stringComp (quux, "foo", 4));
    }
}
