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
 version:            1.0.0
 name:               ame DSP functions
 description:        Header-only DSP library for Cortex-M
 website:            https://github.com/AkiyukiOkayasu/ame
 license:            MIT
 minimumCppStandard: 20

 dependencies:
 
 END_JUCE_MODULE_DECLARATION
 *******************************************************************************/

#pragma once

#include "ame_Ambisonics.hpp"
#include "ame_AudioBuffer.hpp"
#include "ame_Biquad.hpp"
#include "ame_Circuit.hpp"
#include "ame_Conversion.hpp"
#include "ame_DcBlock.hpp"
#include "ame_Delay.hpp"
#include "ame_Math.hpp"
#include "ame_Oscillator.hpp"
#include "ame_Pcm.hpp"
#include "ame_Random.hpp"
#include "ame_RcLowPass.hpp"
#include "ame_Reverb.hpp"
#include "ame_String.hpp"
#include "ame_Util.hpp"