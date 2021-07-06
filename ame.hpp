/** 
    main header file.
    @file ame.hpp
    @author Akiyuki Okayasu (akiyuki.okayasu@gmail.com)
    @copyright Copyright (c) 2021 - Akiyuki Okayasu
    
    AME is released under the MIT license.
*/

/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION
 
 ID:                 ame
 vendor:             Akiyuki Okayasu
 version:            0.0.1
 name:
 description:
 website:
 license:            MIT
 
 dependencies:
 
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include "audio/audioBlockView.hpp"
#include "audio/audioBuffer.hpp"
#include "audio/biquad.hpp"
#include "audio/conversion.hpp"
#include "audio/delay.hpp"
#include "audio/frequency.hpp"
#include "audio/interpolation.hpp"
#include "audio/mapping.hpp"
#include "audio/phase.hpp"
#include "audio/sine_oscillator.hpp"
#include "audio/smoothing.hpp"
#include "audio/time.hpp"
#include "audio/volume.hpp"
#include "audio/wavetable.hpp"
#include "math/constants.hpp"
#include "math/specialFunctions.hpp"
#include "system/bytes.hpp"
#include "system/wrap.hpp"
