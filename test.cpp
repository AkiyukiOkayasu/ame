#define CATCH_CONFIG_MAIN
#include "ame.hpp"

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

        auto ar = ame::make_waveTable<float, 5> (f);

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