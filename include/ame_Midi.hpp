/**
    MIDI
    @file ame_Midi.hpp    
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

#pragma once

#include <cstdint>

//==============================================================================
//Type
namespace ame::midi
{
struct UsbMidiEventPacket
{
    uint8_t header;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
};

/// MIDI note
struct Note
{
    uint8_t noteNumber; ///< [0, 127]
    uint8_t velocity;   ///< [0, 127]
    uint8_t channel;    ///< [0, 15]
};

/// MIDI Control change
struct ControlChange
{
    uint8_t controlNumber; ///< [0, 119]
    uint8_t value;         ///< [0, 127]
    uint8_t channel;       ///< [0, 15]
};
} // namespace ame::midi
