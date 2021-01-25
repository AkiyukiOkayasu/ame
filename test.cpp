#define CATCH_CONFIG_MAIN
#include "ame.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Decibels")
{
    SECTION("decibelsToGain()")
    {
        REQUIRE(ame::Decibels::decibelsToGain(0.0f) == Approx(1.0f));
        REQUIRE(ame::Decibels::decibelsToGain(-6.0f) == Approx(0.501187));
        REQUIRE(ame::Decibels::decibelsToGain(-20.0f) == Approx(0.1));
        REQUIRE(ame::Decibels::decibelsToGain(3.0f) == Approx(1.412538));
    }
    SECTION("gainToDecibels()")
    {
        REQUIRE(ame::Decibels::gainToDecibels(1.0f) == Approx(0.0f));
        REQUIRE(ame::Decibels::gainToDecibels(0.501187f) == Approx(-6.0f));
        REQUIRE(ame::Decibels::gainToDecibels(0.1f) == Approx(-20.0f));
        REQUIRE(ame::Decibels::gainToDecibels(1.412538f) == Approx(3.0f));
    }
}

TEST_CASE("dspHelpers")
{
    SECTION("addModule2Pi()")
    {
        REQUIRE(ame::addModulo2Pi(6.28318530717958647692f, 0.1f) == Approx(0.1f));
        REQUIRE(ame::addModulo2Pi(6.28318530717958647692f, 2.5f) == Approx(2.5f));
    }
    SECTION("freqToPeriod()")
    {
        REQUIRE(ame::freqToPeriod(440.0f) == Approx(0.002272727f));
        REQUIRE(ame::freqToPeriod(44100.0f) == Approx(0.000022676f));
    }
    SECTION("periodToFreq()")
    {
        REQUIRE(ame::periodToFreq(0.002272727f) == Approx(440.0f));
        REQUIRE(ame::periodToFreq(0.000022676f) == Approx(44100.0f));
    }
    SECTION("scale()")
    {
        REQUIRE(ame::scale(0.5f, 0.0f, 1.0f, -100.0f, 100.0f) == Approx(0.0f));
        REQUIRE(ame::scale(0.0f, 0.0f, 1.0f, -100.0f, 100.0f) == Approx(-100.0f));
    }
}

TEST_CASE("MIDI")
{
    SECTION("ftom()") { REQUIRE(ame::MIDI::freqToMidi(440.0f) == Approx(69)); }
    SECTION("mtof()") { REQUIRE(ame::MIDI::midiToFreq(69) == Approx(440.0f)); }
}