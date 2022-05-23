#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Tamtam.hpp"
#include "ame.hpp"
#include "sine440.hpp"

#include <doctest/doctest.h>
#include <iomanip>
#include <span>

using doctest::Approx;

TEST_CASE ("Radian / Degree")
{
    CHECK_EQ (ame::rad2deg (0.0f), Approx (0.0f));
    CHECK_EQ (ame::rad2deg (ame::halfPi<float>), Approx (90.0f));
    CHECK_EQ (ame::rad2deg (ame::pi<float>), Approx (180.0f));
    CHECK_EQ (ame::rad2deg (ame::twoPi<float>), Approx (360.0f));
    CHECK_EQ (ame::deg2rad (0.0f), Approx (0.0f));
    CHECK_EQ (ame::deg2rad (90.0f), Approx (ame::halfPi<float>));
    CHECK_EQ (ame::deg2rad (180.0f), Approx (ame::pi<float>));
    CHECK_EQ (ame::deg2rad (360.0f), Approx (ame::twoPi<float>));
}

TEST_CASE ("Rc lowpass")
{
    ame::dsp::RcLowPass<float> rcf { 48000 }; //sample rate: 48kHz
    rcf.setCutOffFrequency (20.0f);           //LPF cutoff freq: 20Hz
    rcf.process (0.0f);
    float lastOutput = 0.0f;
    for (int i = 0; i < 100; ++i)
    {
        float o = rcf.process (1.0f);
        CHECK_GT (o, Approx (lastOutput));
        lastOutput = o;
    }

    rcf.setRawCoefficient (1.0f); //No filter
    CHECK_EQ (rcf.getRawCoefficient(), Approx (1.0f));
}

TEST_CASE ("Ambisonics")
{
    SUBCASE ("3D")
    {
        ame::dsp::Ambisonics3D1st<float> ambi {};
        ame::dsp::Polar<float> pos { .azimuth = 0.0f, .elevation = 0.0f };
        ame::dsp::encodeAmbisonics1st (0.5f, ambi, pos);
        [[maybe_unused]] const auto f = ame::dsp::decodeAmbisonics1st (ambi, pos);
    }
    SUBCASE ("2D")
    {
        ame::dsp::Ambisonics2D1st<float> ambi {};
        ame::dsp::encodeAmbisonics1st (0.5f, ambi, -30.0f);
        [[maybe_unused]] const auto f = ame::dsp::decodeAmbisonics1st (ambi, -60.0f);
    }
}

TEST_CASE ("DcBlock")
{
    ame::dsp::DcBlock<float, 2> dcblock {};
    ame::AudioBuffer<float, 200000> buf { 2 };
    buf.buffer.fill (0.3f); //Add DC offset
    auto view = buf.makeAudioBlockView();
    dcblock.process (view);
    CHECK_LT (view.view[20], view.view[10]);
    CHECK_LT (view.view[40], view.view[30]);
    CHECK_LT (view.view[110], view.view[100]);
    CHECK_LT (view.view[10000], 0.1f);
    CHECK_LT (view.view[100000], 0.05f);
    CHECK_LT (view.view[150000], 0.02f);
}

TEST_CASE ("Freeverb")
{
    ame::dsp::Freeverb<float, 2, 48000> reverb { 48000 };
    SUBCASE ("params")
    {
        reverb.reset();
        reverb.setSampleRate (44100);
    }
    ame::AudioBuffer<float, 100> buf { 2 };
    auto view = buf.makeAudioBlockView();
    SUBCASE ("process")
    {
        reverb.process (view);
    }
}

TEST_CASE ("LinearSmoothedValue")
{
    ame::LinearSmoothedValue<float> smooth { 0.0f, 10 };
    smooth.setTargetValue (1.0f);
    SUBCASE ("LinearSmoothing")
    {
        CHECK_EQ (smooth.getCurrentValue(), Approx (0.0f));
        CHECK_EQ (smooth.getTargetValue(), Approx (1.0f));
        CHECK_EQ (smooth.getNextValue(), Approx (0.1f));
        CHECK_EQ (smooth.getNextValue(), Approx (0.2f));
        CHECK_EQ (smooth.getNextValue(), Approx (0.3f));
        CHECK_EQ (smooth.getNextValue(), Approx (0.4f));
        CHECK_EQ (smooth.getNextValue(), Approx (0.5f));
        CHECK_EQ (smooth.getNextValue(), Approx (0.6f));
        CHECK_EQ (smooth.getNextValue(), Approx (0.7f));
        CHECK_EQ (smooth.getNextValue(), Approx (0.8f));
        CHECK_EQ (smooth.getNextValue(), Approx (0.9f));
        CHECK (smooth.isSmoothing());
        CHECK_EQ (smooth.getNextValue(), Approx (1.0f));
        CHECK (! smooth.isSmoothing());
        CHECK_EQ (smooth.getNextValue(), Approx (1.0f));
        CHECK_EQ (smooth.getNextValue(), Approx (1.0f));
        smooth.reset (0.33f);
        CHECK_EQ (smooth.getCurrentValue(), Approx (0.33f));
    }
}

TEST_CASE ("Volume")
{
    SUBCASE ("decibelsToGain()")
    {
        REQUIRE (ame::decibelsToGain (0.0f) == Approx (1.0f));
        REQUIRE (ame::decibelsToGain (-6.0f) == Approx (0.501187));
        REQUIRE (ame::decibelsToGain (-20.0f) == Approx (0.1));
        REQUIRE (ame::decibelsToGain (3.0f) == Approx (1.412538));
    }
    SUBCASE ("gainToDecibels()")
    {
        REQUIRE (ame::gainToDecibels (1.0f) == Approx (0.0f));
        REQUIRE (ame::gainToDecibels (0.501187f) == Approx (-6.0f));
        REQUIRE (ame::gainToDecibels (0.1f) == Approx (-20.0f));
        REQUIRE (ame::gainToDecibels (1.412538f) == Approx (3.0f));
    }
}

TEST_CASE ("Time")
{
    SUBCASE ("addModule2Pi()")
    {
        REQUIRE (ame::addModulo2Pi (6.28318530717958647692f, 0.1f) == Approx (0.1f));
        REQUIRE (ame::addModulo2Pi (6.28318530717958647692f, 2.5f) == Approx (2.5f));
    }

    SUBCASE ("freqToPeriod()")
    {
        REQUIRE (ame::freqToPeriod (440.0f) == Approx (0.002272727f));
        REQUIRE (ame::freqToPeriod (44100.0f) == Approx (0.000022676f));
    }

    SUBCASE ("periodToFreq()")
    {
        REQUIRE (ame::periodToFreq (0.002272727f) == Approx (440.0f));
        REQUIRE (ame::periodToFreq (0.000022676f) == Approx (44100.0f));
    }

    SUBCASE ("scale()")
    {
        REQUIRE (ame::scale (0.5f, 0.0f, 1.0f, -100.0f, 100.0f) == Approx (0.0f));
        REQUIRE (ame::scale (0.0f, 0.0f, 1.0f, -100.0f, 100.0f) == Approx (-100.0f));
    }

    SUBCASE ("BPM to ms")
    {
        REQUIRE (ame::bpmToMs (60.0f) == Approx (1000.0f));
        REQUIRE (ame::bpmToMs (120.0f) == Approx (500.0f));
    }

    SUBCASE ("ms to BPM")
    {
        REQUIRE (ame::msToBpm (1000.0f) == Approx (60.0f));
        REQUIRE (ame::msToBpm (500.0f) == Approx (120.0f));
    }
}

TEST_CASE ("Frequency")
{
    SUBCASE ("Convert pol between car")
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
    SUBCASE ("ftom()")
    {
        REQUIRE (ame::freqToMidi (440.0f) == Approx (69));
    }
    SUBCASE ("mtof()")
    {
        REQUIRE (ame::midiToFreq (69) == Approx (440.0f));
    }
}

TEST_CASE ("WaveTable")
{
    SUBCASE ("makeTable()")
    {
        auto f = [] (auto& x)
        {
            x = ame::sin (x * ame::twoPi<float>);
        };

        constexpr auto ar = ame::makeWaveTable<float, 5> (f);

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

    SUBCASE ("sinetable")
    {
        constexpr std::array<float, 512> wavetable = ame::makeSineTable<float, 512>();
        CHECK_EQ (wavetable[0], Approx (0.0f).scale (1));
        CHECK_GT (wavetable[127], 0.97f);
        CHECK_LT (wavetable[256], 0.03f);
        CHECK_GT (wavetable[256], -0.03f);
        CHECK_LT (wavetable[384], -0.97f);
        CHECK_EQ (wavetable[511], Approx (0.0f).scale (1));
    }
}

TEST_CASE ("Oscillator")
{
    SUBCASE ("Wavetable")
    {
        constexpr std::array<float, 512> wavetable = ame::makeSineTable<float, 512>();
        std::span s { wavetable };
        ame::WavetableOscillator osc { s, 44100.0f };
        CHECK_EQ (wavetable[0], Approx (0.0f).scale (1));
        osc.setFrequency (440); //Hz
        CHECK_EQ (osc.nextSample(), Approx (0.0f).scale (1));
        CHECK_GT (osc.nextSample(), 0.001f);
        CHECK_GT (osc.nextSample(), 0.01f);
        CHECK_GT (osc.nextSample(), 0.1f);
    }

    SUBCASE ("SineWaveOsc")
    {
        ame::SineOscillator osc (44100.0f);
        osc.setFrequency (440.0f);
    }
}

TEST_CASE ("Filter")
{
    SUBCASE ("Biquad")
    {
        constexpr int numChannels = 2;
        constexpr int numSamples = 1000;
        std::array<float, numChannels * numSamples> buffer {};
        std::span s { buffer };
        ame::AudioBlockView block (s, numChannels); //Stereo
        ame::dsp::Biquad<float, numChannels> filter { 44100 };

        // LPF
        filter.makeLowPass (100.0f, 0.71f);
        filter.process (block);
        for (int i = 0; i < numSamples * numChannels; ++i)
        {
            REQUIRE (buffer[i] == Approx (0.0f));
        }

        // HPF for remove DC offset;
        filter.makeHighPass (200.0f, 0.71f);
        buffer.fill (0.3f); //DC offset;
        filter.process (block);
        for (int i = 500; i < numSamples * numChannels; ++i)
        {
            REQUIRE_LE (buffer[i], 0.01f);
        }
    }
}

TEST_CASE ("Wrap")
{
    SUBCASE ("Increment")
    {
        ame::Wrap<int32_t> w { 10 };
        CHECK_EQ (w.get(), 0);
        CHECK_EQ (w++, 1);
        CHECK_EQ (++w, 2);
        w += 2;
        CHECK_EQ (w.get(), 4);
        w += 9;
        CHECK_EQ (w.get(), 3);
    }

    SUBCASE ("set")
    {
        ame::Wrap<int32_t> w { 10 };
        w.set (0);
        CHECK_EQ (w.get(), 0);
        w.set (12);
        CHECK_EQ (w.get(), 2);
        w.set (-9);
        CHECK_EQ (w.get(), 1);
    }
}

TEST_CASE ("AudioBuffer")
{
    SUBCASE ("size")
    {
        constexpr int numChannels = 2;
        ame::AudioBuffer<float, 10> buf (numChannels);
        CHECK_EQ (buf.getNumChannels(), numChannels);
        CHECK_EQ (buf.getNumSamplesPerChannel(), 5);
        CHECK_EQ (buf.buffer.size(), 10);
    }

    SUBCASE ("getPeak")
    {
        ame::AudioBuffer<float, 4> buf (2);
        buf.buffer[0] = 0.0f;
        buf.buffer[1] = 0.2f;
        buf.buffer[2] = 0.9f;
        buf.buffer[3] = -0.5f;
        REQUIRE (buf.getPeak (0) == Approx (0.9f));
        REQUIRE (buf.getPeak (1) == Approx (0.5f));
    }

    SUBCASE ("getRMSLevel")
    {
        ame::AudioBuffer<float, 6> buf (1);
        buf.buffer[0] = 0.0f;
        buf.buffer[1] = 0.2f;
        buf.buffer[2] = 0.9f;
        buf.buffer[3] = -0.5f;
        buf.buffer[4] = -0.1f;
        buf.buffer[5] = -0.0f;
        CHECK (buf.getRMSLevel (0) == Approx (0.4301162f).scale (1));
    }

    SUBCASE ("MakeView")
    {
        ame::AudioBuffer<float, 6> buf (1);
        buf.buffer[0] = 0.0f;
        buf.buffer[1] = 0.2f;
        buf.buffer[2] = 0.9f;
        buf.buffer[3] = -0.5f;
        buf.buffer[4] = -0.1f;
        buf.buffer[5] = -0.0f;
        auto view = buf.makeAudioBlockView();
        CHECK_EQ (view.view[0], Approx (0.0f));
        CHECK_EQ (view.view[1], Approx (0.2f));
        CHECK_EQ (view.view[2], Approx (0.9f));
        CHECK_EQ (view.view[3], Approx (-0.5f));
        CHECK_EQ (view.view[4], Approx (-0.1f));
        CHECK_EQ (view.view[5], Approx (-0.0f));
        auto subView = buf.makeAudioBlockView (2, 4);
        CHECK_EQ (subView.getNumSamplesPerChannel(), 4);
        CHECK_EQ (subView.getNumChannels(), 1);
        CHECK_EQ (subView.view[0], Approx (0.9f));
        CHECK_EQ (subView.view[1], Approx (-0.5f));
        CHECK_EQ (subView.view[2], Approx (-0.1f));
        CHECK_EQ (subView.view[3], Approx (-0.0f));
    }
}

TEST_CASE ("AudioBlockView")
{
    std::array<float, 6> v = { 0.0f, 0.2f, 0.9f, -0.5f, -0.1f, -0.0f };
    std::span s { v };
    SUBCASE ("size")
    {
        ame::AudioBlockView block (s, 2);
        CHECK_EQ (block.getNumChannels(), 2);
        CHECK_EQ (block.getNumSamplesPerChannel(), 3);
        CHECK_EQ (block.view.size(), 6);
    }

    SUBCASE ("getPeak")
    {
        ame::AudioBlockView block (s, 2);
        REQUIRE (block.getPeak (0) == Approx (0.9f));
        REQUIRE (block.getPeak (1) == Approx (0.5f));
    }

    SUBCASE ("getRMSLevel")
    {
        ame::AudioBlockView block (s, 1);
        CHECK (block.getRMSLevel (0) == Approx (0.4301162f).scale (1));
    }
}

TEST_CASE ("sine440Reader")
{
    constexpr ame::wav::WavReader sine440Reader (wav::sine440, sizeof (wav::sine440));
    constexpr ame::wav::WavReader tamtamReader (wav::tamtam, sizeof (wav::tamtam));

    SUBCASE ("getFileSize()")
    {
        CHECK_EQ (sine440Reader.getFileSize(), sizeof (wav::sine440) - 8);
        CHECK_EQ (tamtamReader.getFileSize(), sizeof (wav::tamtam) - 8);
    }

    SUBCASE ("sampleRate")
    {
        CHECK_EQ (sine440Reader.getSampleRate(), 44100);
        CHECK_EQ (tamtamReader.getSampleRate(), 44100);
    }

    SUBCASE ("bitRate")
    {
        CHECK_EQ (sine440Reader.getBitRate(), 16);
        CHECK_EQ (tamtamReader.getBitRate(), 16);
    }

    SUBCASE ("Channel")
    {
        CHECK_EQ (sine440Reader.getNumChannels(), 1);
        CHECK_EQ (tamtamReader.getNumChannels(), 2);
    }

    SUBCASE ("numSample")
    {
        CHECK_EQ (sine440Reader.getNumSamples(), 88200);
        CHECK_EQ (tamtamReader.getNumSamples(), 279290);
    }

    SUBCASE ("getDataPointer")
    {
        const auto* data = sine440Reader.getDataPointer();
        CHECK_EQ (data[0], 0x00);
        CHECK_EQ (data[1], 0x00);
        CHECK_EQ (data[2], 0x6B);
        CHECK_EQ (data[3], 0x06);
        CHECK_EQ (data[4], 0xCC);
        CHECK_EQ (data[5], 0x0C);
        CHECK_EQ (data[6], 0x28);
        CHECK_EQ (data[7], 0x13);

        const auto* tamtamData = tamtamReader.getDataPointer();
        CHECK_EQ (tamtamData[0], 0x00);
        CHECK_EQ (tamtamData[1], 0x00);
        CHECK_EQ (tamtamData[2], 0x00);
        CHECK_EQ (tamtamData[3], 0x00);
        CHECK_EQ (tamtamData[4], 0x00);
        CHECK_EQ (tamtamData[5], 0x00);
        CHECK_EQ (tamtamData[6], 0x00);
        CHECK_EQ (tamtamData[7], 0x00);
        CHECK_EQ (tamtamData[8], 0xFF);
        CHECK_EQ (tamtamData[9], 0xFF);
        CHECK_EQ (tamtamData[10], 0xFF);
        CHECK_EQ (tamtamData[11], 0xFF);
        CHECK_EQ (tamtamData[12], 0x00);
        CHECK_EQ (tamtamData[13], 0x00);
        CHECK_EQ (tamtamData[14], 0xFF);
        CHECK_EQ (tamtamData[15], 0xFF);
    }
}

TEST_CASE ("WavPlayer")
{
    SUBCASE ("Sine")
    {
        //Mono 16bit LittleEndian 44.1kHz
        std::array<float, 100> v {};
        constexpr int numChannel = 1;
        std::span s { v };
        ame::AudioBlockView block (s, numChannel);
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

    SUBCASE ("Tamtam")
    {
        //Stereo 16bit LittleEndian 44.1kHz
        std::array<float, 10> v {};
        constexpr int numChannel = 2;
        std::span s { v };
        ame::AudioBlockView block (s, numChannel);
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

///@todo
TEST_CASE ("Byte")
{
    SUBCASE ("makeByte")
    {
    }
}

TEST_CASE ("String")
{
    SUBCASE ("Comp")
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
    SUBCASE ("noise")
    {
        for (auto i = 0; i < 100; ++i)
        {
            auto f = ame::random();
            REQUIRE (0.0f <= f);
            REQUIRE (f <= 1.0f);
        }
    }
}

TEST_CASE ("Delay")
{
    SUBCASE ("small int delay")
    {
        constexpr int numChannel = 1;
        constexpr int bufferSize = 4;
        constexpr int maximumDelaySamples = 100;
        std::array<float, numChannel * bufferSize> v;
        std::span s { v };
        ame::AudioBlockView block (s, numChannel);
        ame::dsp::Delay<numChannel, maximumDelaySamples> delay;
        delay.setDelay (2); //2sample delay
        block.clear();
        block.setSample (0, 0, 1.0);
        delay.process (block);

        CHECK (v[0] == Approx (0.0f).scale (1));
        CHECK (v[1] == Approx (0.0f).scale (1));
        CHECK (v[2] == Approx (1.0f).scale (1));
        CHECK (v[3] == Approx (0.0f).scale (1));
    }

    SUBCASE ("large int delay")
    {
        constexpr int numChannel = 1;
        constexpr int bufferSize = 4;
        constexpr int maximumDelaySamples = 100;
        std::array<float, numChannel * bufferSize> v;
        std::span s { v };
        ame::AudioBlockView block (s, numChannel);
        ame::dsp::Delay<numChannel, maximumDelaySamples> delay;
        delay.setDelay (10); //10sample delay
        block.clear();
        block.setSample (0, 0, 1.0); //Impulse

        delay.process (block);
        CHECK (v[0] == Approx (0.0f).scale (1));
        CHECK (v[1] == Approx (0.0f).scale (1)); //1sample delay
        CHECK (v[2] == Approx (0.0f).scale (1));
        CHECK (v[3] == Approx (0.0f).scale (1));
        block.clear();
        delay.process (block);
        CHECK (v[0] == Approx (0.0f).scale (1)); //4sample delay
        CHECK (v[1] == Approx (0.0f).scale (1));
        CHECK (v[2] == Approx (0.0f).scale (1));
        CHECK (v[3] == Approx (0.0f).scale (1));
        delay.process (block);
        CHECK (v[0] == Approx (0.0f).scale (1)); //8sample delay
        CHECK (v[1] == Approx (0.0f).scale (1));
        CHECK (v[2] == Approx (1.0f).scale (1)); //10sample delay
        CHECK (v[3] == Approx (0.0f).scale (1));
    }

    SUBCASE ("fractional delay")
    {
        constexpr int numChannel = 1;
        constexpr int bufferSize = 4;
        constexpr int maximumDelaySamples = 100;
        std::array<float, numChannel * bufferSize> v {};
        std::span s { v };
        ame::AudioBlockView block (s, numChannel);
        ame::dsp::Delay<numChannel, maximumDelaySamples> delay;
        delay.setDelay (1.5f); //1.5sample delay
        block.clear();
        block.setSample (0, 0, 1.0); //Impulse

        delay.process (block);
        CHECK (v[0] == Approx (0.0f).scale (1));
        CHECK (v[1] == Approx (0.5f).scale (1));
        CHECK (v[2] == Approx (0.5f).scale (1));
        CHECK (v[3] == Approx (0.0f).scale (1));
    }
}