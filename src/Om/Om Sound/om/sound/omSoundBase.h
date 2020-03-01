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

#ifndef INCLUDE_OM_SOUND_BASE_H
#define INCLUDE_OM_SOUND_BASE_H


#include "base/omSoundBaseConfig.h"


// Sample Information
#include "base/omSoundInt24.h"
#include "base/omSoundSample.h"
#include "base/omSoundSampleType.h"


// Speaker/Channel Management
#include "base/omSoundChannelType.h"
#include "base/omSoundChannelLayout.h"
#include "base/omSoundChannelLayoutType.h"
#include "base/omSoundPanDirection.h"

// Sound Buffer Management Classes
#include "base/omSoundBuffer.h"
#include "base/omSoundSharedBufferPool.h"
#include "base/omSoundSharedSoundBuffer.h"

// Streaming Sound I/O Classes
#include "base/omSoundInputStream.h"
#include "base/omSoundOutputStream.h"
#include "base/omSoundDataInputStream.h"

// Sound Resource Management
#include "base/omSoundFormat.h"
#include "base/omSound.h"

// Response Classes
#include "base/omSoundDirectionalIR.h"
#include "base/omSoundFrequencyData.h"
#include "base/omSoundFrequencyBands.h"
#include "base/omSoundSIMDCrossover.h"

// MIDI Classes
#include "base/omSoundMIDIMessage.h"
#include "base/omSoundMIDIEvent.h"
#include "base/omSoundMIDITime.h"
#include "base/omSoundTimeSignature.h"
#include "base/omSoundMIDIBuffer.h"

// Sound Frame
#include "base/omSoundFrame.h"
#include "base/omSoundResult.h"

#include "base/omSoundLerpState.h"


#endif // INCLUDE_OM_SOUND_BASE_H
