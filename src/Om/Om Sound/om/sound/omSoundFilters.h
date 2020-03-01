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


// Equalization Filters
#include "filters/omSoundCutoffFilter.h"


// Reverb Filters
#include "filters/omSoundReverbFilter.h"


// Sound Sources/Destinations
#include "filters/omSoundPlayer.h"


// Dynamics Filters
#include "filters/omSoundCompressor.h"
#include "filters/omSoundLimiter.h"
#include "filters/omSoundHDRFilter.h"


// Sample Rate Conversion
#include "filters/omSoundResampler.h"


#endif // INCLUDE_OM_SOUND_FILTERS_H
