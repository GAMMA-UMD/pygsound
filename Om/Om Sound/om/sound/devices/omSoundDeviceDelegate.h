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

#ifndef INCLUDE_OM_SOUND_DEVICE_DELEGATE_H
#define INCLUDE_OM_SOUND_DEVICE_DELEGATE_H


#include "omSoundDevicesConfig.h"


//##########################################################################################
//*************************  Start Om Sound Devices Namespace  *****************************
OM_SOUND_DEVICES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


class SoundDevice;




/// Define a short name for the type of the function object that is used to recieve audio from SoundDevice objects.
/**
  * The implementor of the function can read the specified number of samples from the input buffer and
  * use them in some way.
  *
  * The given time represents the absolute time of the first sample in the buffer,
  * measured relative to the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
  */
typedef Function<void ( SoundDevice& device, const SoundBuffer& inputBuffer, Size numInputSamples, const Time& time )> SoundInputCallback;




/// Define a short name for the type of the function object that is used to send audio to SoundDevice objects for output.
/**
  * The implementor should write the specified number of samples to the output buffer for each channel
  * and then return the number of samples that were successfully written to the output buffer.
  *
  * The given time represents the absolute time of the first sample in the buffer,
  * measured relative to the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
  */
typedef Function<Size ( SoundDevice& device, SoundBuffer& outputBuffer, Size numOutputSamples, const Time& time )> SoundOutputCallback;




//********************************************************************************
/// A class that contains function objects that recieve SoundDevice events.
/**
  * Any device-related event that might be processed has an appropriate callback
  * function object. Each callback function is called by the device
  * whenever such an event is received. If a callback function in the delegate
  * is not initialized, the device simply ignores it.
  */
class SoundDeviceDelegate
{
	public:
		
		//********************************************************************************
		//******	Sound Device Delegate Callback Functions
			
			
			/// A function object that is called whenever the device provides input audio.
			/**
			  * This callback is called whenever the SoundDevice has audio that it has captured on its inputs.
			  * The implementor of the callback method can read the samples from the given input sound buffer.
			  *
			  * This method will be called from a separate thread (the audio processing thread)
			  * so any data accessed in the callback function should use proper thread synchonization
			  * to avoid unsafe access. The input callback for a device is always called before the output callback.
			  */
			SoundInputCallback inputCallback;
			
			
			/// A function object that is called whenever the device requests output audio.
			/**
			  * This callback is called whenever the SoundDevice needs output audio to send to the device.
			  * The implementor of the callback method should produce sound that is written to the output
			  * buffer for the requested number of samples. If the callback takes too long to process
			  * the samples for output, the result is that output buffers are dropped, producing choppy audio.
			  *
			  * This method will be called from a separate thread (the audio processing thread)
			  * so any data accessed in the callback function should use proper thread synchonization
			  * to avoid unsafe access. The output callback for a device is always called after the input callback.
			  */
			SoundOutputCallback outputCallback;
			
			
			/// A function called whenever the device detects that the user has taken too long to process audio I/O.
			/**
			  * When this happens, the device must drop input or output frames, causing glitchy audio.
			  * This callback exists so that the user can detect this event and reduce the audio thread
			  * processing load.
			  */
			Function<void ( SoundDevice& device )> processOverload;
			
			
			/// A function object that is called whenever the sound device is removed from the system.
			Function<void ( SoundDevice& device )> removed;
			
			
			/// A function object that is called whenever the sampling rate for a SoundDevice has changed.
			Function<void ( SoundDevice& device, SampleRate newSampleRate )> sampleRateChanged;
			
			
			
};




//##########################################################################################
//*************************  End Om Sound Devices Namespace  *******************************
OM_SOUND_DEVICES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


#endif // INCLUDE_OM_SOUND_DEVICE_DELEGATE_H
