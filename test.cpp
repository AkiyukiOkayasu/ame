#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "audio/decibel.h"

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
        REQUIRE(ame::Decibels::gainToDecibels(0.501187) == Approx(-6.0f));
        REQUIRE(ame::Decibels::gainToDecibels(0.1f) == Approx(-20.0f));
        REQUIRE(ame::Decibels::gainToDecibels(1.412538) == Approx(3.0f));
    }
}