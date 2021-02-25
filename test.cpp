#define CATCH_CONFIG_MAIN
#include "ame.hpp"

#include <catch2/catch.hpp>
#include <iomanip>

TEST_CASE ("Decibels")
{
    SECTION ("decibelsToGain()")
    {
        REQUIRE (ame::dBToGain (0.0f) == Approx (1.0f));
        REQUIRE (ame::dBToGain (-6.0f) == Approx (0.501187));
        REQUIRE (ame::dBToGain (-20.0f) == Approx (0.1));
        REQUIRE (ame::dBToGain (3.0f) == Approx (1.412538));
    }
    SECTION ("gainToDecibels()")
    {
        REQUIRE (ame::gainTodB (1.0f) == Approx (0.0f));
        REQUIRE (ame::gainTodB (0.501187f) == Approx (-6.0f));
        REQUIRE (ame::gainTodB (0.1f) == Approx (-20.0f));
        REQUIRE (ame::gainTodB (1.412538f) == Approx (3.0f));
    }
}

TEST_CASE ("dspHelpers")
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
        auto f = [] (auto& x) { x = ame::sinf (x * ame::twoPi); };
        auto ar = ame::make_waveTable<float, 5> (f);
        for (auto&& e : ar)
        {
            std::cout << std::fixed << std::setprecision (5) << e << std::endl;
        }

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