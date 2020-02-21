/*
 * Project:     Om Software
 * Version:     1.0.0
 * Website:     http://www.carlschissler.com/om
 * Author(s):   Carl Schissler
 * 
 * Copyright (c) 2016, Carl Schissler
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 	1. Redistributions of source code must retain the above copyright
 * 	   notice, this list of conditions and the following disclaimer.
 * 	2. Redistributions in binary form must reproduce the above copyright
 * 	   notice, this list of conditions and the following disclaimer in the
 * 	   documentation and/or other materials provided with the distribution.
 * 	3. Neither the name of the copyright holder nor the
 * 	   names of its contributors may be used to endorse or promote products
 * 	   derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INCLUDE_OM_SOUND_FILTERS_H
#define INCLUDE_OM_SOUND_FILTERS_H


#include "filters/omSoundFiltersConfig.h"


// Base Framework Classes
#include "filters/omSoundFilterVersion.h"
#include "filters/omSoundFilterParameterInfo.h"
#include "filters/omSoundFilterParameterType.h"
#include "filters/omSoundFilterParameterValue.h"
#include "filters/omSoundFilterParameterFlags.h"
#include "filters/omSoundFilterParameter.h"
#include "filters/omSoundFilterState.h"
#include "filters/omSoundFilterPreset.h"
#include "filters/omSoundFilter.h"
#include "filters/omSoundFilterInputStream.h"


// Routing and Mixing Filters
#include "filters/omSoundMixer.h"
#include "filters/omSoundSplitter.h"
#include "filters/omSoundFilterSeries.h"
#include "filters/omSoundFilterGraph.h"


// Panning and Channel Mixing Filters
#include "filters/omSoundPanner.h"
#include "filters/omSoundMonoMixer.h"
#include "filters/omSoundMonoSplitter.h"


// Equalization Filters
#include "filters/omSoundDCFilter.h"
#include "filters/omSoundCutoffFilter.h"
#include "filters/omSoundBandFilter.h"
#include "filters/omSoundShelfFilter.h"
#include "filters/omSoundParametricFilter.h"
#include "filters/omSoundCrossover.h"
#include "filters/omSoundParametricEqualizer.h"
#include "filters/omSoundGraphicEqualizer.h"
#include "filters/omSoundConvolutionFilter.h"


// Delay Filters
#include "filters/omSoundDelay.h"
#include "filters/omSoundMultichannelDelay.h"


// Reverb Filters
#include "filters/omSoundReverbFilter.h"


// Sound Sources/Destinations
#include "filters/omSoundToneGenerator.h"
#include "filters/omSoundStreamPlayer.h"
#include "filters/omSoundStreamRecorder.h"
#include "filters/omSoundThreadedStreamRecorder.h"
#include "filters/omSoundPlayer.h"


// Dynamics Filters
#include "filters/omSoundGainFilter.h"
#include "filters/omSoundCompressor.h"
#include "filters/omSoundLimiter.h"
#include "filters/omSoundExpander.h"
#include "filters/omSoundHDRFilter.h"


// Distortion Filters
#include "filters/omSoundDistortion.h"
#include "filters/omSoundSaturator.h"
#include "filters/omSoundBitcrusher.h"


// Modulation Filters
#include "filters/omSoundTremolo.h"
#include "filters/omSoundVibrato.h"
#include "filters/omSoundFlanger.h"


// Sample Rate Conversion / Pitch Shift Filters
#include "filters/omSoundResampler.h"
#include "filters/omSoundPitchShifter.h"


// Analysis Filters
#include "filters/omSoundIRUtility.h"


#endif // INCLUDE_OM_SOUND_FILTERS_H
