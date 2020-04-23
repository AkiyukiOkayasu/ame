/*
 * Copyright 2020, Akiyuki Okayasu
 */

#ifndef _NXPLIB_AUDIO_CONVERTER_H_
#define _NXPLIB_AUDIO_CONVERTER_H_

#include "../nxplib.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*
convertFloatToUInt16
 */
static inline uint32_t convertFloatToUInt16(float in) {
    // TODO 実装
    // https://github.com/juce-framework/JUCE/blob/master/modules/juce_audio_basics/buffers/juce_AudioDataConverters.cpp
    return (uint32_t)in;
}



#if defined(__cplusplus)
}
#endif

#endif /* _NXPLIB_AUDIO_CONVERTER_H_ */
