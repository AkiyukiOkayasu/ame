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
 version:            0.0.11
 name:               ame DSP functions
 description:        Header-only DSP library for Cortex-M
 website:            https://github.com/AkiyukiOkayasu/ame
 license:            MIT
 minimumCppStandard: 17

 dependencies:
 
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include "audio/ame_Conversion.hpp"
#include "audio/audioBlockView.hpp"
#include "audio/audioBuffer.hpp"
#include "audio/biquad.hpp"
#include "audio/delay.hpp"
#include "audio/interpolation.hpp"
#include "audio/mapping.hpp"
#include "audio/phase.hpp"
#include "audio/random.hpp"
#include "audio/sine_oscillator.hpp"
#include "audio/smoothing.hpp"
#include "audio/wavPlayer.hpp"
#include "audio/wavReader.hpp"
#include "audio/wavetable.hpp"
#include "math/constants.hpp"
#include "math/specialFunctions.hpp"
#include "system/bytes.hpp"
#include "system/stringUtilities.hpp"
#include "system/wrap.hpp"