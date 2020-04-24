/*
 * Copyright 2020, Akiyuki Okayasu
 */

#ifndef _NXPLIB_AUDIO_CONVERTER_H_
#define _NXPLIB_AUDIO_CONVERTER_H_

#include "../nxplib.h"

#if defined(__cplusplus)
extern "C" {
#endif
#include <math.h>

#define M_TWOPI 6.28318530717958647692528676655900576

/*
convertFloatToUInt16
juce::AudioDataConverters::convertFloatToInt16BE
https://github.com/juce-framework/JUCE/blob/master/modules/juce_audio_basics/buffers/juce_AudioDataConverters.cpp
 */
static inline uint32_t convertFloatToUInt16(const float source) {    
    // TODO　範囲丸め
    return (uint32_t)(source * INT16_MAX);
}

/*
addModulo2Pi
位相の足し算
0~2piの範囲でラップされる
*/
static inline float addModulo2Pi(float phase, const float increment) {
    phase += increment;
    if (phase > M_TWOPI) {
        phase -= M_TWOPI;
    }
    return phase;
}


#if defined(__cplusplus)
}
#endif

#endif /* _NXPLIB_AUDIO_CONVERTER_H_ */
