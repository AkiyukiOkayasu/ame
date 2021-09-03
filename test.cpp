#define CATCH_CONFIG_MAIN
#include "ame.hpp"
#include "resource/Tamtam.hpp"
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

TEST_CASE ("sine440Reader")
{
    constexpr ame::wav::WavReader sine440Reader (wav::sine440, sizeof (wav::sine440));
    constexpr ame::wav::WavReader tamtamReader (wav::tamtam, sizeof (wav::tamtam));

    SECTION ("getFileSize()")
    {
        REQUIRE (sine440Reader.getFileSize() == sizeof (wav::sine440) - 8);
        REQUIRE (tamtamReader.getFileSize() == sizeof (wav::tamtam) - 8);
    }

    SECTION ("sampleRate")
    {
        REQUIRE (sine440Reader.getSampleRate() == 44100);
        REQUIRE (tamtamReader.getSampleRate() == 44100);
    }

    SECTION ("bitRate")
    {
        REQUIRE (sine440Reader.getBitRate() == 16);
        REQUIRE (tamtamReader.getBitRate() == 16);
    }

    SECTION ("Channel")
    {
        REQUIRE (sine440Reader.getNumChannels() == 1);
        REQUIRE (tamtamReader.getNumChannels() == 2);
    }

    SECTION ("numSample")
    {
        REQUIRE (sine440Reader.getNumSamples() == 88200);
        REQUIRE (tamtamReader.getNumSamples() == 279290);
    }

    SECTION ("getDataPointer")
    {
        auto data = sine440Reader.getDataPointer();
        REQUIRE (data[0] == 0x00);
        REQUIRE (data[1] == 0x00);
        REQUIRE (data[2] == 0x6B);
        REQUIRE (data[3] == 0x06);
        REQUIRE (data[4] == 0xCC);
        REQUIRE (data[5] == 0x0C);
        REQUIRE (data[6] == 0x28);
        REQUIRE (data[7] == 0x13);

        auto tamtamData = tamtamReader.getDataPointer();
        REQUIRE (tamtamData[0] == 0x00);
        REQUIRE (tamtamData[1] == 0x00);
        REQUIRE (tamtamData[2] == 0x00);
        REQUIRE (tamtamData[3] == 0x00);
        REQUIRE (tamtamData[4] == 0x00);
        REQUIRE (tamtamData[5] == 0x00);
        REQUIRE (tamtamData[6] == 0x00);
        REQUIRE (tamtamData[7] == 0x00);
        REQUIRE (tamtamData[8] == 0xFF);
        REQUIRE (tamtamData[9] == 0xFF);
        REQUIRE (tamtamData[10] == 0xFF);
        REQUIRE (tamtamData[11] == 0xFF);
        REQUIRE (tamtamData[12] == 0x00);
        REQUIRE (tamtamData[13] == 0x00);
        REQUIRE (tamtamData[14] == 0xFF);
        REQUIRE (tamtamData[15] == 0xFF);
    }
}

TEST_CASE ("WavPlayer")
{
    SECTION ("Sine")
    {
        //Mono 16bit LittleEndian 44.1kHz
        float v[100] = {};
        constexpr int numChannel = 1;
        ame::AudioBlockView<float> block (v, numChannel, 100);
        constexpr ame::wav::WavReader reader (wav::sine440, sizeof (wav::sine440));
        ame::wav::WavPlayer player (reader);
        player.play();
        player.process (block);
        REQUIRE (v[0] == Approx (0.00000f).scale (1));
        REQUIRE (v[1] == Approx (0.05014f).scale (1));
        REQUIRE (v[2] == Approx (0.09998f).scale (1));
        REQUIRE (v[3] == Approx (0.14966f).scale (1));
        REQUIRE (v[4] == Approx (0.19843f).scale (1));
        REQUIRE (v[5] == Approx (0.24677f).scale (1));
        REQUIRE (v[6] == Approx (0.29376f).scale (1));
        REQUIRE (v[7] == Approx (0.34000f).scale (1));
        REQUIRE (v[8] == Approx (0.38452f).scale (1));
        REQUIRE (v[9] == Approx (0.42786f).scale (1));
        REQUIRE (v[10] == Approx (0.46930f).scale (1));
        REQUIRE (v[11] == Approx (0.50894f).scale (1));
        REQUIRE (v[12] == Approx (0.54666f).scale (1));
        REQUIRE (v[13] == Approx (0.58215f).scale (1));
        REQUIRE (v[14] == Approx (0.61539f).scale (1));
        REQUIRE (v[15] == Approx (0.64621f).scale (1));
        REQUIRE (v[16] == Approx (0.67450f).scale (1));
        REQUIRE (v[17] == Approx (0.70007f).scale (1));
        REQUIRE (v[18] == Approx (0.72302f).scale (1));
        REQUIRE (v[19] == Approx (0.74298f).scale (1));
        REQUIRE (v[20] == Approx (0.76016f).scale (1));
        REQUIRE (v[21] == Approx (0.77426f).scale (1));
        REQUIRE (v[22] == Approx (0.78537f).scale (1));
        REQUIRE (v[23] == Approx (0.79330f).scale (1));
        REQUIRE (v[24] == Approx (0.79834f).scale (1));
        REQUIRE (v[25] == Approx (0.79987f).scale (1));
        REQUIRE (v[26] == Approx (0.79874f).scale (1));
        REQUIRE (v[27] == Approx (0.79398f).scale (1));
        REQUIRE (v[28] == Approx (0.78647f).scale (1));
        REQUIRE (v[29] == Approx (0.77567f).scale (1));
        REQUIRE (v[30] == Approx (0.76190f).scale (1));
        REQUIRE (v[31] == Approx (0.74509f).scale (1));
        REQUIRE (v[32] == Approx (0.72543f).scale (1));
        REQUIRE (v[33] == Approx (0.70285f).scale (1));
        REQUIRE (v[34] == Approx (0.67749f).scale (1));
        REQUIRE (v[35] == Approx (0.64963f).scale (1));
        REQUIRE (v[36] == Approx (0.61896f).scale (1));
        REQUIRE (v[37] == Approx (0.58606f).scale (1));
        REQUIRE (v[38] == Approx (0.55081f).scale (1));
        REQUIRE (v[39] == Approx (0.51331f).scale (1));
        REQUIRE (v[40] == Approx (0.47400f).scale (1));
        REQUIRE (v[41] == Approx (0.43250f).scale (1));
        REQUIRE (v[42] == Approx (0.38968f).scale (1));
        REQUIRE (v[43] == Approx (0.34497f).scale (1));
        REQUIRE (v[44] == Approx (0.29926f).scale (1));
        REQUIRE (v[45] == Approx (0.25198f).scale (1));
        REQUIRE (v[46] == Approx (0.20410f).scale (1));
        REQUIRE (v[47] == Approx (0.15509f).scale (1));
        REQUIRE (v[48] == Approx (0.10577f).scale (1));
        REQUIRE (v[49] == Approx (0.05573f).scale (1));
        REQUIRE (v[50] == Approx (0.00577f).scale (1));
        REQUIRE (v[51] == Approx (-0.04446f).scale (1));
        REQUIRE (v[52] == Approx (-0.09442f).scale (1));
        REQUIRE (v[53] == Approx (-0.14386f).scale (1));
        REQUIRE (v[54] == Approx (-0.19312f).scale (1));
        REQUIRE (v[55] == Approx (-0.24115f).scale (1));
        REQUIRE (v[56] == Approx (-0.28857f).scale (1));
        REQUIRE (v[57] == Approx (-0.33478f).scale (1));
        REQUIRE (v[58] == Approx (-0.37952f).scale (1));
        REQUIRE (v[59] == Approx (-0.42303f).scale (1));
        REQUIRE (v[60] == Approx (-0.46466f).scale (1));
        REQUIRE (v[61] == Approx (-0.50458f).scale (1));
        REQUIRE (v[62] == Approx (-0.54242f).scale (1));
        REQUIRE (v[63] == Approx (-0.57828f).scale (1));
        REQUIRE (v[64] == Approx (-0.61169f).scale (1));
        REQUIRE (v[65] == Approx (-0.64285f).scale (1));
        REQUIRE (v[66] == Approx (-0.67145f).scale (1));
        REQUIRE (v[67] == Approx (-0.69724f).scale (1));
        REQUIRE (v[68] == Approx (-0.72061f).scale (1));
        REQUIRE (v[69] == Approx (-0.74084f).scale (1));
        REQUIRE (v[70] == Approx (-0.75836f).scale (1));
        REQUIRE (v[71] == Approx (-0.77280f).scale (1));
        REQUIRE (v[72] == Approx (-0.78427f).scale (1));
        REQUIRE (v[73] == Approx (-0.79257f).scale (1));
        REQUIRE (v[74] == Approx (-0.79788f).scale (1));
        REQUIRE (v[75] == Approx (-0.79996f).scale (1));
        REQUIRE (v[76] == Approx (-0.79889f).scale (1));
        REQUIRE (v[77] == Approx (-0.79474f).scale (1));
        REQUIRE (v[78] == Approx (-0.78748f).scale (1));
        REQUIRE (v[79] == Approx (-0.77704f).scale (1));
        REQUIRE (v[80] == Approx (-0.76361f).scale (1));
        REQUIRE (v[81] == Approx (-0.74719f).scale (1));
        REQUIRE (v[82] == Approx (-0.72775f).scale (1));
        REQUIRE (v[83] == Approx (-0.70560f).scale (1));
        REQUIRE (v[84] == Approx (-0.68051f).scale (1));
        REQUIRE (v[85] == Approx (-0.65289f).scale (1));
        REQUIRE (v[86] == Approx (-0.62256f).scale (1));
        REQUIRE (v[87] == Approx (-0.58997f).scale (1));
        REQUIRE (v[88] == Approx (-0.55490f).scale (1));
        REQUIRE (v[89] == Approx (-0.51770f).scale (1));
        REQUIRE (v[90] == Approx (-0.47849f).scale (1));
        REQUIRE (v[91] == Approx (-0.43741f).scale (1));
        REQUIRE (v[92] == Approx (-0.39453f).scale (1));
        REQUIRE (v[93] == Approx (-0.35019f).scale (1));
        REQUIRE (v[94] == Approx (-0.30444f).scale (1));
        REQUIRE (v[95] == Approx (-0.25748f).scale (1));
        REQUIRE (v[96] == Approx (-0.20956f).scale (1));
        REQUIRE (v[97] == Approx (-0.16074f).scale (1));
        REQUIRE (v[98] == Approx (-0.11133f).scale (1));
        REQUIRE (v[99] == Approx (-0.06146f).scale (1));
    }

    SECTION ("Tamtam")
    {
        //Stereo 16bit LittleEndian 44.1kHz
        float v[10] = {};
        constexpr int numChannel = 2;
        ame::AudioBlockView<float> block (v, numChannel, 5);
        constexpr ame::wav::WavReader reader (wav::tamtam, sizeof (wav::tamtam));
        ame::wav::WavPlayer player (reader);
        player.play();
        player.process (block);
        REQUIRE (v[0] == Approx (0.00000f).scale (1));
        REQUIRE (v[1] == Approx (0.00000f).scale (1));
        REQUIRE (v[2] == Approx (0.00000f).scale (1));
        REQUIRE (v[3] == Approx (0.00000f).scale (1));
        REQUIRE (v[4] == Approx (-0.00003f).scale (1));
        REQUIRE (v[5] == Approx (-0.00003f).scale (1));
        REQUIRE (v[6] == Approx (0.00000f).scale (1));
        REQUIRE (v[7] == Approx (-0.00003f).scale (1));
        REQUIRE (v[8] == Approx (-0.00006f).scale (1));
        REQUIRE (v[9] == Approx (-0.00009f).scale (1));
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

TEST_CASE ("Random")
{
    SECTION ("noise")
    {
        for (auto i = 0; i < 100; ++i)
        {
            auto f = ame::random();
            REQUIRE (0.0f <= f);
            REQUIRE (f <= 1.0f);
        }
    }
}